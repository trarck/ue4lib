// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "TestSceneComponent.h"

// Sets default values for this component's properties
UTestSceneComponent::UTestSceneComponent()
	:LaserPointerLightRadius(20.0f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UTestSceneComponent::CreateComponents()
{
	AActor* Owner = GetOwner();

	// Laser pointer
	{
		LaserPointerMeshComponent = NewObject<UStaticMeshComponent>(Owner, TEXT("LaserPointerMeshComponent"));
		LaserPointerMeshComponent->SetupAttachment(this);


		UStaticMesh* LaserPointerMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LaserPointer/LaserPointerMesh"));
		if (LaserPointerMesh != nullptr)
		{
			LaserPointerMeshComponent->SetStaticMesh(LaserPointerMesh);
		}

		LaserPointerMeshComponent->SetMobility(EComponentMobility::Movable);
		LaserPointerMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		UMaterialInstance* LaserPointerMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/LaserPointer/LaserPointerMaterialInst"));
		if (LaserPointerMaterialInst)
		{
		UMaterialInstanceDynamic* LaserPointerMID = UMaterialInstanceDynamic::Create(LaserPointerMaterialInst, GetTransientPackage());
		if (LaserPointerMID)
		{
		LaserPointerMeshComponent->SetMaterial(0, LaserPointerMID);
		}
		}

		UMaterialInstance* TranslucentLaserPointerMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/LaserPointer/TranslucentLaserPointerMaterialInst"));
		if (TranslucentLaserPointerMaterialInst)
		{
		UMaterialInstanceDynamic* TranslucentLaserPointerMID = UMaterialInstanceDynamic::Create(TranslucentLaserPointerMaterialInst, GetTransientPackage());
		if (TranslucentLaserPointerMID) {
		LaserPointerMeshComponent->SetMaterial(1, TranslucentLaserPointerMID);
		}
		}
	}

	// Hover cue for laser pointer
	{
		HoverMeshComponent = NewObject<UStaticMeshComponent>(Owner,TEXT("HoverMeshComponent"));
		HoverMeshComponent->SetupAttachment(this);

		UStaticMesh* HoverMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LaserPointer/HoverMesh"));
		if (HoverMesh)
		{
			HoverMeshComponent->SetStaticMesh(HoverMesh);
		}

		HoverMeshComponent->SetMobility(EComponentMobility::Movable);
		HoverMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Add a light!
		{
			HoverPointLightComponent = NewObject<UPointLightComponent>(Owner,TEXT("HoverPointLightComponent"));
			HoverPointLightComponent->SetupAttachment(HoverMeshComponent);

			HoverPointLightComponent->SetLightColor(FLinearColor::Red);
			HoverPointLightComponent->SetIntensity(30.0f);
			HoverPointLightComponent->SetMobility(EComponentMobility::Movable);
			HoverPointLightComponent->SetAttenuationRadius(LaserPointerLightRadius);
			HoverPointLightComponent->bUseInverseSquaredFalloff = false;
			HoverPointLightComponent->SetCastShadows(false);
		}
	}
}

// Called when the game starts
void UTestSceneComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTestSceneComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

