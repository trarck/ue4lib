// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "RayCaster.h"

#include "RayInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogRayCaster, Log, All);

// Sets default values for this component's properties
URayCaster::URayCaster()
	:LaserPointerRadius(0.5f),
	LaserPointerHoverBallRadius(1.5f),
	LaserPointerLightRadius(20.0f),
	LaserPointerLightPullBackDistance(2.5f),
	bPenetrate(false),
	WorldScaleFactor(1.0f),
	LaserPointerMeshComponent(nullptr),
	HoverMeshComponent(nullptr),
	HoverPointLightComponent(nullptr),
	LaserPointerMID(nullptr),
	TranslucentLaserPointerMID(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	//PrimaryComponentTick.bCanEverTick = true;
}

void URayCaster::CreateLaserPointerMesh()
{
	UE_LOG(LogRayCaster, Log, TEXT("Create Laser Pointer Mesh"));
	AActor* Owner = GetOwner();

	// Laser pointer
	LaserPointerMeshComponent = NewObject<UStaticMeshComponent>(Owner, TEXT("LaserPointerMeshComponent"));
	LaserPointerMeshComponent->SetupAttachment(this);
	LaserPointerMeshComponent->RegisterComponent();


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
		LaserPointerMID = UMaterialInstanceDynamic::Create(LaserPointerMaterialInst, GetTransientPackage());
		if (LaserPointerMID)
		{
			LaserPointerMeshComponent->SetMaterial(0, LaserPointerMID);
		}
	}

	UMaterialInstance* TranslucentLaserPointerMaterialInst = LoadObject<UMaterialInstance>(nullptr, TEXT("/Game/LaserPointer/TranslucentLaserPointerMaterialInst"));
	if (TranslucentLaserPointerMaterialInst)
	{
		TranslucentLaserPointerMID = UMaterialInstanceDynamic::Create(TranslucentLaserPointerMaterialInst, GetTransientPackage());
		if (TranslucentLaserPointerMID) {
			LaserPointerMeshComponent->SetMaterial(1, TranslucentLaserPointerMID);
		}
	}
}


void URayCaster::CreateHoverMesh()
{
	UE_LOG(LogRayCaster, Log, TEXT("Create Hover Mesh"));
	// Hover cue for laser pointer
	HoverMeshComponent = NewObject<UStaticMeshComponent>(GetOwner(), TEXT("HoverMeshComponent"));
	HoverMeshComponent->SetupAttachment(this);
	HoverMeshComponent->RegisterComponent();

	UStaticMesh* HoverMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/LaserPointer/HoverMesh"));
	if (HoverMesh)
	{
		HoverMeshComponent->SetStaticMesh(HoverMesh);
	}

	HoverMeshComponent->SetMobility(EComponentMobility::Movable);
	HoverMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HoverMeshComponent->SetVisibility(false);
}

void URayCaster::CreateHoverPointLight()
{

	UE_LOG(LogRayCaster, Log, TEXT("Create Hover Point Light"));
	// Add a light!
	HoverPointLightComponent = NewObject<UPointLightComponent>(GetOwner(), TEXT("HoverPointLightComponent"));

	if (HoverMeshComponent)
	{
		HoverPointLightComponent->SetupAttachment(HoverMeshComponent);
	}

	HoverPointLightComponent->RegisterComponent();
	HoverPointLightComponent->SetLightColor(FLinearColor::Red);
	HoverPointLightComponent->SetIntensity(30.0f);
	HoverPointLightComponent->SetMobility(EComponentMobility::Movable);
	HoverPointLightComponent->SetAttenuationRadius(LaserPointerLightRadius);
	HoverPointLightComponent->bUseInverseSquaredFalloff = false;
	HoverPointLightComponent->SetCastShadows(false);
	HoverPointLightComponent->SetVisibility(false);
}

void URayCaster::LoadFromChildren()
{
	const TArray<USceneComponent*>& Children = GetAttachChildren();
	for (int i = 0; i < Children.Num(); ++i)
	{
		if (Children[i]->ComponentHasTag(FName(TEXT("LaserPoint"))))
		{
			LaserPointerMeshComponent = Cast<UStaticMeshComponent>(Children[i]);
			continue;
		}

		if (Children[i]->ComponentHasTag(FName(TEXT("HoverPoint"))))
		{
			HoverMeshComponent = Cast<UStaticMeshComponent>(Children[i]);
			continue;
		}
	}

	if (HoverMeshComponent)
	{
		const TArray<USceneComponent*>& HoverChildren = HoverMeshComponent->GetAttachChildren();
		for (int i = 0; i < HoverChildren.Num(); ++i)
		{
			if (HoverChildren[i]->ComponentHasTag(FName(TEXT("HoverPointLight"))))
			{
				HoverPointLightComponent = Cast<UPointLightComponent>(HoverChildren[i]);
				break;
			}
		}
	}	
}

// Called when the game starts
void URayCaster::BeginPlay()
{
	Super::BeginPlay();

	WorldScaleFactor = GetWorld()->GetWorldSettings()->WorldToMeters / 100.0f;

	LoadFromChildren();

	if (LaserPointerMeshComponent == nullptr)
	{
		CreateLaserPointerMesh();
	}
	else
	{
		LaserPointerMID = UMaterialInstanceDynamic::Create(LaserPointerMeshComponent->GetMaterial(0), GetTransientPackage());
		LaserPointerMeshComponent->SetMaterial(0, LaserPointerMID);

		TranslucentLaserPointerMID = UMaterialInstanceDynamic::Create(LaserPointerMeshComponent->GetMaterial(1), GetTransientPackage());
		LaserPointerMeshComponent->SetMaterial(1, TranslucentLaserPointerMID);

		UE_LOG(LogRayCaster, Log, TEXT("BeginPlay:%d,%d"),LaserPointerMID==nullptr,TranslucentLaserPointerMID==nullptr);
	}

	if (HoverMeshComponent == nullptr)
	{
		CreateHoverMesh();
	}

	if (HoverPointLightComponent == nullptr)
	{
		CreateHoverPointLight();
	}
}

void URayCaster::RegisterProcess(URayInput* RayInput)
{
	if (RayInput)
	{
		UE_LOG(LogRayCaster, Log, TEXT("RegisterProcess"));
		RayInput->OnProcessRay.AddDynamic(this, &URayCaster::ProcessRayHit);
	}
}

void URayCaster::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bHaveRay)
{
	if (bHaveRay)
	{
		FVector WantEnd = bPenetrate || !bHit ? End : HitResult.ImpactPoint;

		const FVector LaserPointerDelta = WantEnd - Start;

		float LaserPointerLength = FMath::Max(0.000001f, LaserPointerDelta.Size());

		float LaserPointScale = LaserPointerRadius * WorldScaleFactor * 2.0f;
		float HoverPointScale = LaserPointerHoverBallRadius * WorldScaleFactor * 2.0f;

		if (bHit && !bPenetrate)
		{
			LaserPointScale *= 0.35f;
			HoverPointScale *= 0.35f;
		}

		LaserPointerMeshComponent->SetVisibility(true);
		LaserPointerMeshComponent->SetRelativeScale3D(FVector(LaserPointerLength, LaserPointScale, LaserPointScale));

		if (bHit)
		{
			HoverMeshComponent->SetRelativeScale3D(FVector(HoverPointScale));
			HoverMeshComponent->SetVisibility(true);
			HoverMeshComponent->SetWorldLocation(HitResult.ImpactPoint);

			HoverPointLightComponent->SetVisibility(bPenetrate);
			HoverPointLightComponent->SetAttenuationRadius(LaserPointerLightRadius);

			const float PullBackAmount = LaserPointerLightPullBackDistance * WorldScaleFactor;
			HoverPointLightComponent->SetWorldLocation(HitResult.ImpactPoint - PullBackAmount * LaserPointerDelta.GetSafeNormal());
		}
		else
		{
			HoverMeshComponent->SetVisibility(false);
			HoverPointLightComponent->SetVisibility(false);
		}

		SetLaserVisuals(FLinearColor(1.0f,0,0,1.0f));

		static FName StaticLengthParameterName("Length");
		if (LaserPointerMID)
		{
			LaserPointerMID->SetScalarParameterValue(StaticLengthParameterName, LaserPointerLength);
		}

		if (TranslucentLaserPointerMID)
		{
			TranslucentLaserPointerMID->SetScalarParameterValue(StaticLengthParameterName, LaserPointerLength);
		}
	}
	else
	{
		LaserPointerMeshComponent->SetVisibility(false);
		HoverMeshComponent->SetVisibility(false);
		HoverPointLightComponent->SetVisibility(false);
	}
}

void URayCaster::SetLaserVisuals(const FLinearColor& NewColor, const float CrawlFade, const float CrawlSpeed)
{
	static FName StaticLaserColorParameterName("LaserColor");
	LaserPointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);
	TranslucentLaserPointerMID->SetVectorParameterValue(StaticLaserColorParameterName, NewColor);

	static FName StaticCrawlParameterName("Crawl");
	LaserPointerMID->SetScalarParameterValue(StaticCrawlParameterName, CrawlFade);
	TranslucentLaserPointerMID->SetScalarParameterValue(StaticCrawlParameterName, CrawlFade);

	static FName StaticCrawlSpeedParameterName("CrawlSpeed");
	LaserPointerMID->SetScalarParameterValue(StaticCrawlSpeedParameterName, CrawlSpeed);
	TranslucentLaserPointerMID->SetScalarParameterValue(StaticCrawlSpeedParameterName, CrawlSpeed);

	HoverPointLightComponent->SetLightColor(NewColor);
}