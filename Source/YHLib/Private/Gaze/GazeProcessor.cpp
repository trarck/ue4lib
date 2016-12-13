// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazeProcessor.h"

#include "RayInput.h"
#include "RayInteractiveComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRayCaster, Log, All);

// Sets default values for this component's properties
UGazeProcessor::UGazeProcessor()
	:bPenetrate(true),
	PointerHoverRadius(1.5f),
	Duration(0.5f),
	GazeColor(FLinearColor::Blue),
	Elapsed(0),
	PointerMeshComponent(nullptr),
	HoverMeshComponent(nullptr),
	PointerMID(nullptr),
	HoverMID(nullptr),
	bChangeColor(false),
	State(EGazeSelectState::None)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGazeProcessor::CreatePointerMesh()
{
	// Laser pointer
	PointerMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("LaserPointerMeshComponent"));
#if ENGINE_MAJOR_VERSION ==4 && ENGINE_MINOR_VERSION >=12
	PointerMeshComponent->SetupAttachment(this);
#else
	PointerMeshComponent->AttachParent = this;
#endif
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

void UGazeProcessor::CreateHoverMesh()
{
	UE_LOG(LogRayCaster, Log, TEXT("Create Hover Mesh"));
	// Hover cue for laser pointer
	HoverMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("HoverMeshComponent"));
#if ENGINE_MAJOR_VERSION ==4 && ENGINE_MINOR_VERSION >=12
	HoverMeshComponent->SetupAttachment(this);
#else
	HoverMeshComponent->AttachParent = this;
#endif
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

void UGazeProcessor::LoadFromChildren()
{
#if ENGINE_MAJOR_VERSION ==4 && ENGINE_MINOR_VERSION >=12
	const TArray<USceneComponent*>& Children = GetAttachChildren();
#else
	const TArray<USceneComponent*>& Children = AttachChildren;
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
void UGazeProcessor::BeginPlay()
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
	SetHoverPercent(1.0f);
}

void UGazeProcessor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (State == EGazeSelectState::Hover)
	{
		Elapsed += DeltaTime;
		float Percent = Elapsed / Duration;

		if (Percent >= 1.0f)
		{
			Percent = 1.0f;
			State = EGazeSelectState::Hovered;
		}
		float Scale = 1 + 0.5f*Percent;
		HoverMeshComponent->SetRelativeScale3D(FVector(Scale, Scale, Scale));
	}
}

void UGazeProcessor::RegisterProcess(URayInput* RayInput)
{
	if (RayInput)
	{
		RayInput->OnProcessRay.AddDynamic(this, &UGazeProcessor::ProcessRayHit);
	}
}

void UGazeProcessor::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay)
{
	if (bHaveRay)
	{
		bool bShowPointer = true;

		//face to Start point
		FRotator Rotator = FRotationMatrix::MakeFromX(Start - End).Rotator();
		Rotator.Pitch -= 90;

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

				if (RayInteractiveComponent->IsHover())
				{
					//is hover start
					if (bBeginHit || (RayInteractiveComponent->IsHoverChanged() && State == EGazeSelectState::None))
					{
						//set gaze color
						if (RayInteractiveComponent->HaveHoverColor())
						{
							bChangeColor = true;
							SetLaserVisuals(RayInteractiveComponent->GetHoverColor());
						}
						else
						{
							SetLaserVisuals(GazeColor);
						}
						//start or restart hover
						StartStay();
					}

					//show hover
					if (State >= EGazeSelectState::Hover)
					{
						HoverMeshComponent->SetVisibility(true);
						HoverMeshComponent->SetWorldLocation(HitResult.ImpactPoint);
						HoverMeshComponent->SetWorldRotation(Rotator);
					}
					bShowPointer = false;
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

		if (bShowPointer)
		{
			//set pointer visible
			PointerMeshComponent->SetVisibility(true);

			PointerMeshComponent->SetWorldRotation(Rotator);

			//update pointer end position
			PointerMeshComponent->SetWorldLocation(PointPosition);
		}
		else
		{
			PointerMeshComponent->SetVisibility(false);
		}
	}
	else
	{
		//No ray
		PointerMeshComponent->SetVisibility(false);
		EndHover();
	}
}

void UGazeProcessor::StartStay()
{
	//UE_LOG(LogRayCaster, Log, TEXT("StartStay"));
	State = EGazeSelectState::Hover;
	Elapsed = 0;
	HoverMeshComponent->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void UGazeProcessor::EndHover()
{
	if (State >= EGazeSelectState::Hover)
	{
		HoverMeshComponent->SetVisibility(false);
	}

	if (bChangeColor)
	{
		SetLaserVisuals(GazeColor);
		bChangeColor = false;
	}

	State = EGazeSelectState::None;
}

void UGazeProcessor::SetLaserVisuals(const FLinearColor& NewColor)
{
	static FName StaticLaserColorParameterName("Color");
	PointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
	HoverMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
}

void UGazeProcessor::SetHoverPercent(float Percent)
{
	static FName StaticLaserPercentParameterName("Percent");
	HoverMID->SetScalarParameterValue(StaticLaserPercentParameterName, Percent);
}