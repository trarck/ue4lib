// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazePointer.h"

#include "RayInput.h"
#include "RayInteractiveComponent.h"
#include "GazeActionComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRayCaster, Log, All);

// Sets default values for this component's properties
UGazePointer::UGazePointer()
	:bPenetrate(true),
	PointerHoverRadius(1.5f),
	ActionDuration(0.5f),
	StayDuration(0.5f),
	GazeColor(FLinearColor::Blue),
	Elapsed(0),
	bIsHover(false),
	Duration(1),
	PointerMeshComponent(nullptr),
	HoverMeshComponent(nullptr),
	PointerMID(nullptr),
	HoverMID(nullptr),
	GazeActionComponent(nullptr),
	bChangeColor(false),
	bShowHover(false),
	bShowHoverable(false),
	State(EGazeState::None)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGazePointer::CreatePointerMesh()
{
	// Laser pointer
	PointerMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("LaserPointerMeshComponent"));
	PointerMeshComponent->SetupAttachment(this);
	PointerMeshComponent->RegisterComponent();


	UStaticMesh* LaserPointerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/Geometry/SmallQuad"));
	if (LaserPointerMesh != nullptr)
	{
		PointerMeshComponent->SetStaticMesh(LaserPointerMesh);
	}

	PointerMeshComponent->SetMobility(EComponentMobility::Movable);
	PointerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UMaterialInstance* LaserPointerMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/Materials/CirclelInst"));
	if (LaserPointerMaterialInst)
	{
		PointerMID = UMaterialInstanceDynamic::Create(LaserPointerMaterialInst, GetTransientPackage());
		if (PointerMID)
		{
			PointerMeshComponent->SetMaterial(0, PointerMID);
		}
	}
}

void UGazePointer::CreateHoverMesh()
{
	UE_LOG(LogRayCaster, Log, TEXT("Create Hover Mesh"));
	// Hover cue for laser pointer
	HoverMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("HoverMeshComponent"));
	HoverMeshComponent->SetupAttachment(this);
	HoverMeshComponent->RegisterComponent();

	UStaticMesh* HoverMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Meshes/Geometry/Quad"));
	if (HoverMesh)
	{
		HoverMeshComponent->SetStaticMesh(HoverMesh);
	}

	HoverMeshComponent->SetMobility(EComponentMobility::Movable);
	HoverMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoverMeshComponent->SetVisibility(false);

	UMaterialInstance* HoverMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/Materials/Radial/RadialRing"));
	if (HoverMaterialInst)
	{
		HoverMID = UMaterialInstanceDynamic::Create(HoverMaterialInst, GetTransientPackage());
		if (HoverMID)
		{
			HoverMeshComponent->SetMaterial(0, HoverMID);
		}
	}
}

void UGazePointer::LoadFromChildren()
{
	const TArray<USceneComponent*>& Children = GetAttachChildren();
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
				UE_LOG(LogRayCaster, Log, TEXT("Have Hover Material"));
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
void UGazePointer::BeginPlay()
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

void UGazePointer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == EGazeState::Stay)
	{
		Elapsed += DeltaTime;
		if (Elapsed >= StayDuration)
		{
			State = EGazeState::Hover;
			Elapsed -= StayDuration;
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
			if (GazeActionComponent && GazeActionComponent->IsValidLowLevel())
			{
				GazeActionComponent->OnAction.Broadcast();
			}
		}
		SetHoverPercent(Percent);
	}
}

void UGazePointer::RegisterProcess(URayInput* RayInput)
{
	if (RayInput)
	{
		RayInput->OnProcessRay.AddDynamic(this, &UGazePointer::ProcessRayHit);
	}
}

void UGazePointer::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay)
{
	if (bHaveRay)
	{
		//set pointer visible
		PointerMeshComponent->SetVisibility(true);

		//face to Start point
		FRotator Rotator = FRotationMatrix::MakeFromX(Start - End).Rotator();
		Rotator.Pitch -= 90;
		PointerMeshComponent->SetWorldRotation(Rotator);

		//UE_LOG(LogRayCaster, Log, TEXT("bHit:%d"), bHit);
		FVector PointPosition = End;

		if (bHit)
		{
			bool bProtrudeThrough = bPenetrate;

			URayInteractiveComponent* RayInteractiveComponent = HitResult.GetActor()->FindComponentByClass<URayInteractiveComponent>();

			//check should response gaze
			if (RayInteractiveComponent)
			{
				bProtrudeThrough = RayInteractiveComponent->IsProtrudeThrough();

				if (bBeginHit)
				{
					//set current action component
					this->GazeActionComponent = HitResult.GetActor()->FindComponentByClass<UGazeActionComponent>();
				}

				if (RayInteractiveComponent->IsHover())
				{
					//show hover
					if (State >= EGazeState::Hover)
					{
						HoverMeshComponent->SetVisibility(true);
						HoverMeshComponent->SetWorldLocation(HitResult.ImpactPoint);
						HoverMeshComponent->SetWorldRotation(Rotator);
					}

					//is hover start
					if (bBeginHit || RayInteractiveComponent->IsHoverChanged())
					{
						//set gaze style
						if (GazeActionComponent)
						{
							//use target gaze style
							//set Action duration
							float ComponentActionDuration = GazeActionComponent->GetActionDuration();
							if (ComponentActionDuration != 0)
							{
								Duration = ComponentActionDuration;
							}
							else
							{
								Duration = ActionDuration;
							}

							//set gaze color
							if (GazeActionComponent->HaveGazeColor())
							{
								bChangeColor = true;
								SetLaserVisuals(GazeActionComponent->GetGazeColor());
							}
							else
							{
								SetLaserVisuals(GazeColor);
							}
						}

						//start or restart hover
						StartStay();
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

void UGazePointer::StartStay()
{
	//UE_LOG(LogRayCaster, Log, TEXT("StartStay"));
	bIsHover = true;
	State =EGazeState::Stay;
	//bShowHover = true;
	Elapsed = 0;
	SetHoverPercent(0);
}

void UGazePointer::EndHover()
{
	bIsHover = false;
	if (bShowHover)
	{
		bShowHover = false;
		HoverMeshComponent->SetVisibility(false);
		if (bChangeColor)
		{
			SetLaserVisuals(GazeColor);
			bChangeColor = false;
		}
	}

	State = EGazeState::None;
}

void UGazePointer::SetLaserVisuals(const FLinearColor& NewColor)
{
	static FName StaticLaserColorParameterName("Color");
	PointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
	HoverMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
}

void UGazePointer::SetHoverPercent(float Percent)
{
	static FName StaticLaserPercentParameterName("Percent");
	HoverMID->SetScalarParameterValue(StaticLaserPercentParameterName, Percent);
}