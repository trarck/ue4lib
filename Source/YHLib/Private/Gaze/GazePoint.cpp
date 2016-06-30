// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazePoint.h"

#include "RayInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogRayCaster, Log, All);

// Sets default values for this component's properties
UGazePoint::UGazePoint()
	:PointerHoverRadius(1.5f),
	PointerMeshComponent(nullptr),
	HoverMeshComponent(nullptr),
	PointerMID(nullptr),
	HoverMID(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	//PrimaryComponentTick.bCanEverTick = true;
}

void UGazePoint::CreatePointerMesh()
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

void UGazePoint::CreateHoverMesh()
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

void UGazePoint::LoadFromChildren()
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
void UGazePoint::BeginPlay()
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

void UGazePoint::RegisterProcess(URayInput* RayInput)
{
	if (RayInput)
	{
		RayInput->OnProcessRay.AddDynamic(this, &UGazePoint::ProcessRayHit);
	}
}

void UGazePoint::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bHaveRay)
{
	if (bHaveRay)
	{

		PointerMeshComponent->SetVisibility(true);

		if (bHit)
		{
			FRotator Rotator = FRotationMatrix::MakeFromX(Start - HitResult.ImpactPoint).Rotator();
			Rotator.Pitch -= 90;

			PointerMeshComponent->SetWorldLocation(HitResult.ImpactPoint);
			PointerMeshComponent->SetWorldRotation(Rotator);

			HoverMeshComponent->SetVisibility(true);
			HoverMeshComponent->SetWorldLocation(HitResult.ImpactPoint);
			HoverMeshComponent->SetWorldRotation(Rotator);
		}
		else
		{
			FRotator Rotator = FRotationMatrix::MakeFromX(Start - End).Rotator();
			Rotator.Pitch -= 90;
			
			PointerMeshComponent->SetWorldLocation(End);
			PointerMeshComponent->SetWorldRotation(Rotator );

			HoverMeshComponent->SetVisibility(false);
		}
	}
	else
	{
		PointerMeshComponent->SetVisibility(false);
		HoverMeshComponent->SetVisibility(false);
	}
}

void UGazePoint::SetLaserVisuals(const FLinearColor& NewColor)
{
	static FName StaticLaserColorParameterName("Color");
	PointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
	HoverMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
}