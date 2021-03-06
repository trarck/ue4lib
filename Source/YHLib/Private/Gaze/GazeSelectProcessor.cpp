// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazeSelectProcessor.h"
#include "GazeDefine.h"

#include "RayInput.h"
#include "RayInteractiveComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGazeSelectProcessor, Log, All);

// Sets default values for this component's properties
UGazeSelectProcessor::UGazeSelectProcessor()
	:bPenetrate(true),
	PointerHoverRadius(1.5f),
	ActionDuration(0.5f),
	StayDuration(0.5f),
	GazeColor(FLinearColor::Blue),
	Elapsed(0),
	Duration(1),
	PointerMeshComponent(nullptr),
	HoverMeshComponent(nullptr),
	PointerMID(nullptr),
	HoverMID(nullptr),
	GazeInteractiveComponent(nullptr),
	bChangeColor(false),
	State(EGazeState::None)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGazeSelectProcessor::CreatePointerMesh()
{
	// Laser pointer
	PointerMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("LaserPointerMeshComponent"));
#if ENGINE_VERSION_EG_412
	PointerMeshComponent->SetupAttachment(this);
#else
	PointerMeshComponent->AttachParent = this;
#endif
	PointerMeshComponent->RegisterComponent();


	UStaticMesh* LaserPointerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/YHLib/Meshes/Geometry/SM_Quad"));
	if (LaserPointerMesh != nullptr)
	{
		PointerMeshComponent->SetStaticMesh(LaserPointerMesh);
	}

	PointerMeshComponent->SetMobility(EComponentMobility::Movable);
	PointerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UMaterialInstance* LaserPointerMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/Materials/MI_Circle"));
	if (LaserPointerMaterialInst)
	{
		PointerMID = UMaterialInstanceDynamic::Create(LaserPointerMaterialInst, GetTransientPackage());
		if (PointerMID)
		{
			PointerMeshComponent->SetMaterial(0, PointerMID);
		}
	}
}

void UGazeSelectProcessor::CreateHoverMesh()
{
	UE_LOG(LogGazeSelectProcessor, Log, TEXT("Create Hover Mesh"));
	// Hover cue for laser pointer
	HoverMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("HoverMeshComponent"));
#if ENGINE_VERSION_EG_412
	HoverMeshComponent->SetupAttachment(this);
#else
	HoverMeshComponent->AttachParent = this;
#endif
	HoverMeshComponent->RegisterComponent();

	UStaticMesh* HoverMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/YHLib/Meshes/Geometry/SM_Quad"));
	if (HoverMesh)
	{
		HoverMeshComponent->SetStaticMesh(HoverMesh);
	}

	HoverMeshComponent->SetMobility(EComponentMobility::Movable);
	HoverMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoverMeshComponent->SetVisibility(false);

	UMaterialInstance* HoverMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/YHLib/Materials/Radial/MI_RadialRing"));
	if (HoverMaterialInst)
	{
		HoverMID = UMaterialInstanceDynamic::Create(HoverMaterialInst, GetTransientPackage());
		if (HoverMID)
		{
			HoverMeshComponent->SetMaterial(0, HoverMID);
		}
	}
}

void UGazeSelectProcessor::LoadFromChildren()
{
#if ENGINE_VERSION_EG_412
	const TArray<USceneComponent*>& Children = GetAttachChildren();
#else
	const TArray<USceneComponent*>& Children = this->AttachChildren;
#endif
	for (int i = 0; i < Children.Num(); ++i)
	{
		if (Children[i]->ComponentHasTag(FName(TEXT("LaserPoint"))))
		{
			PointerMeshComponent = Cast<UStaticMeshComponent>(Children[i]);
			UMaterialInterface* PointerMaterialInst = PointerMeshComponent->GetMaterial(0);
			if (PointerMaterialInst)
			{
				PointerMID = UMaterialInstanceDynamic::Create(PointerMaterialInst, GetTransientPackage());
				if (PointerMID)
				{
					PointerMeshComponent->SetMaterial(0, PointerMID);
				}
			}
			continue;
		}

		if (Children[i]->ComponentHasTag(FName(TEXT("HoverPoint"))))
		{
			HoverMeshComponent = Cast<UStaticMeshComponent>(Children[i]);
			UMaterialInterface* HoverMaterialInst = HoverMeshComponent->GetMaterial(0);
			if (HoverMaterialInst)
			{
				UE_LOG(LogGazeSelectProcessor, Log, TEXT("Have Hover Material"));
				HoverMID = UMaterialInstanceDynamic::Create(HoverMaterialInst, GetTransientPackage());
				if (HoverMID)
				{
					HoverMeshComponent->SetMaterial(0, HoverMID);
				}
			}
			continue;
		}
	}
}

// Called when the game starts
void UGazeSelectProcessor::BeginPlay()
{
	Super::BeginPlay();

	LoadFromChildren();

	if (PointerMeshComponent == nullptr)
	{
		CreatePointerMesh();
	}

	if (HoverMeshComponent == nullptr)
	{
		CreateHoverMesh();
	}
}

void UGazeSelectProcessor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == EGazeState::Stay)
	{
		Elapsed += DeltaTime;
		if (Elapsed >= StayDuration)
		{
			State = EGazeState::Hover;
			Elapsed -= StayDuration;
			DoHoverStart();
		}
	}
	else if (State == EGazeState::Hover)
	{
		Elapsed += DeltaTime;
		float Percent = Elapsed / Duration;

		if (Percent >= 1.0f)
		{
			Percent = 1.0f;
			State = EGazeState::Actioned;			
			//RunAction
			DoHoverEnd();
		}
		SetHoverPercent(Percent);
	}
}

void UGazeSelectProcessor::RegisterProcess(URayInput* RayInput)
{
	this->RefRayInput = RayInput;
	if (RayInput)
	{
		RayInput->OnProcessRay.AddDynamic(this, &UGazeSelectProcessor::ProcessRayHit);
	}
}

void UGazeSelectProcessor::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay)
{
	if (bHaveRay)
	{
		//set pointer visible
		PointerMeshComponent->SetVisibility(true);

		//face to Start point
		FRotator Rotator = FRotationMatrix::MakeFromX(Start - End).Rotator();
		Rotator.Pitch -= 90;
		PointerMeshComponent->SetWorldRotation(Rotator);

		//UE_LOG(LogGazeSelectProcessor, Log, TEXT("bHit:%d"), bHit);
		FVector PointPosition = End;

		if (bHit)
		{
			bool bProtrudeThrough = bPenetrate;
			GazeInteractiveComponent = HitResult.GetActor()->FindComponentByClass<URayInteractiveComponent>();

			//check should response gaze
			if (GazeInteractiveComponent)
			{
				bProtrudeThrough = GazeInteractiveComponent->IsProtrudeThrough();

				//if (bBeginHit)
				//{
				//	//set current action component
				//	this->GazeInteractiveComponent = HitResult.GetActor()->FindComponentByClass<UGazeInteractiveComponent>();
				//}
				bool bShowHover = GazeInteractiveComponent->IsShowDefaultHover();
				if (GazeInteractiveComponent->IsHover() /*&& GazeInteractiveComponent->IsShowDefaultHover()*/)
				{
					//is hover start
					if (bBeginHit || GazeInteractiveComponent->IsHoverChanged())
					{
						//set gaze style
						if (GazeInteractiveComponent)
						{
							//use target gaze style
							//set Action duration
							float ComponentActionDuration = GazeInteractiveComponent->GetActionDuration();
							if (ComponentActionDuration != 0)
							{
								Duration = ComponentActionDuration;
							}
							else
							{
								Duration = ActionDuration;
							}
						}

						//set gaze color
						if (GazeInteractiveComponent->HaveHoverColor())
						{
							bChangeColor = true;
							SetLaserVisuals(GazeInteractiveComponent->GetHoverColor());
						}
						else
						{
							SetLaserVisuals(GazeColor);
						}

						//start or restart hover
						StartStay(bShowHover);
					}

					//show hover
					if (State >= EGazeState::Hover && bShowHover)
					{
						HoverMeshComponent->SetVisibility(true);
						HoverMeshComponent->SetWorldLocation(HitResult.ImpactPoint);
						HoverMeshComponent->SetWorldRotation(Rotator);
					}
				}
				else
				{
					bProtrudeThrough = true;
					EndHover();
				}
			}
			else
			{
				EndHover();
			}

			//set position to hit
			//use End position can keep Pointer size
			if (!bProtrudeThrough)
			{
				PointPosition = HitResult.ImpactPoint;
			}
		}
		else
		{
			//not hit anything.
			//hide hover
			EndHover();
		}

		//update pointer end position
		PointerMeshComponent->SetWorldLocation(PointPosition);
	}
	else
	{
		//No ray
		PointerMeshComponent->SetVisibility(false);
		EndHover();
	}
}

void UGazeSelectProcessor::StartStay(bool bShowHover)
{
	//UE_LOG(LogGazeSelectProcessor, Log, TEXT("StartStay"));
	State =EGazeState::Stay;
	Elapsed = 0;
	if (bShowHover) 
	{
		SetHoverPercent(0);
	}	
}

void UGazeSelectProcessor::EndHover()
{
	if(State == EGazeState::None) return;
	
	//UE_LOG(LogGazeSelectProcessor, Log, TEXT("EndHover state:%d"),(int)State);
	if (State >=EGazeState::Hover)
	{
		HoverMeshComponent->SetVisibility(false);
	}

	if (bChangeColor)
	{
		SetLaserVisuals(GazeColor);
		bChangeColor = false;
	}

	if (State == EGazeState::Hover)
	{
		DoHoverEnd();
	}

	State = EGazeState::None;
}

void UGazeSelectProcessor::SetLaserVisuals(const FLinearColor& NewColor)
{
	static FName StaticLaserColorParameterName("Color");
	PointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
	HoverMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
}

void UGazeSelectProcessor::SetHoverPercent(float Percent)
{
	static FName StaticLaserPercentParameterName("Percent");
	HoverMID->SetScalarParameterValue(StaticLaserPercentParameterName, Percent);
}

void UGazeSelectProcessor::DoHoverStart_Implementation()
{
	if (RefRayInput && RefRayInput->IsValidLowLevel())
	{
		RefRayInput->PressPointerKey(EKeys::LeftMouseButton);
	}

	//if (GazeInteractiveComponent && GazeInteractiveComponent->IsValidLowLevel())
	//{
	//	//GazeInteractiveComponent->KeyDown(EKeys::LeftMouseButton);
	//	//GazeInteractiveComponent->KeyUp(EKeys::LeftMouseButton);
	//	GazeInteractiveComponent->KeyDown(EKeys::Enter,RefRayInput);
	//	//GazeInteractiveComponent->KeyUp(EKeys::Enter);
	//}
}

void UGazeSelectProcessor::DoHoverEnd_Implementation()
{
	if (RefRayInput && RefRayInput->IsValidLowLevel())
	{
		RefRayInput->ReleasePointerKey(EKeys::LeftMouseButton);
	}
	//if (GazeInteractiveComponent && GazeInteractiveComponent->IsValidLowLevel())
	//{
	//	//GazeInteractiveComponent->KeyDown(EKeys::LeftMouseButton);
	//	//GazeInteractiveComponent->KeyUp(EKeys::LeftMouseButton);
	//	//GazeInteractiveComponent->KeyDown(EKeys::Enter);
	//	GazeInteractiveComponent->KeyUp(EKeys::Enter, RefRayInput);
	//}
}