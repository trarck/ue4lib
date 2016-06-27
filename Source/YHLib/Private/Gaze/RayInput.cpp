// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "RayInput.h"

#include "RayInteractiveComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogRayInput, Log, All);

// Sets default values
URayInput::URayInput()
	:Caster(nullptr),
	LastHitComponent(nullptr),
	RayLength(1000)
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogRayInput, Log, TEXT("RayInput Construct"));
}

void URayInput::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogRayInput, Log, TEXT("RayInput BeginPlay"));
}


// Called every frame
void URayInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//UE_LOG(LogRayInput, Log, TEXT("RayInput Tick"));
	// ...
	Process();
}

void URayInput::Process()
{
	//GLog->Log("Ray Process\n");
	FVector RayStart;
	FVector RayEnd;
	FHitResult HitResult;

	if (GetRayPointer(RayStart, RayEnd))
	{
		//ignore caster
		//TArray<AActor*> ignores;

		//if (Caster != nullptr)
		//{
		//	ignores.Push(Caster->GetOwner());
		//}		

		//check hit actor
		if (GetHitResult(RayStart, RayEnd, DefaultIgnores, HitResult))
		{
			UActorComponent* CurrentComponent = HitResult.GetComponent();

			if (LastHitComponent != CurrentComponent)
			{
				if (LastHitComponent != nullptr)
				{
					//last response exit
					URayInteractiveComponent* LastRayInteractiveComponent = LastHitComponent->GetOwner()->FindComponentByClass<URayInteractiveComponent>();
					if (LastRayInteractiveComponent != nullptr)
					{
						LastRayInteractiveComponent->OnRayExit(LastHitComponent);
					}
				}

				//current enter
				URayInteractiveComponent* RayInteractiveComponent = HitResult.GetActor()->FindComponentByClass<URayInteractiveComponent>();
				if (RayInteractiveComponent != nullptr)
				{
					//UE_LOG(LogRayInput, Log, TEXT("RayInput[%s] Have"), *(HitResult.GetComponent()->GetName()));
					RayInteractiveComponent->OnRayEnter(HitResult.ImpactPoint, CurrentComponent, HitResult);
				}				
				else
				{
					//UE_LOG(LogRayInput, Log, TEXT("RayInput[%s] No"), *(HitResult.GetComponent()->GetName()));
				}

				LastHitComponent = CurrentComponent;
			}
			else
			{
				//current stay
				URayInteractiveComponent* RayInteractiveComponent = HitResult.GetActor()->FindComponentByClass<URayInteractiveComponent>();
				if (RayInteractiveComponent != nullptr)
				{
					RayInteractiveComponent->OnRayStay(HitResult.ImpactPoint, CurrentComponent, HitResult);
				}
			}
			//LastHitPoint = HitResult.ImpactPoint;
		}
		else if (LastHitComponent != nullptr)
		{
			
			//last response exit
			URayInteractiveComponent* LastRayInteractiveComponent = LastHitComponent->GetOwner()->FindComponentByClass<URayInteractiveComponent>();
			if (LastRayInteractiveComponent != nullptr)
			{
				LastRayInteractiveComponent->OnRayExit(LastHitComponent);
			}

			LastHitComponent = nullptr;
		}
	}
}

bool URayInput::GetRayPointer(FVector& RayStart, FVector& RayEnd)
{
	if (Caster != nullptr && Caster->IsValidLowLevel())
	{
		const FTransform Transform = Caster->ComponentToWorld;
		FVector ForwardVector = Transform.GetRotation().GetForwardVector();
		RayStart = Transform.GetLocation();
		RayEnd = RayStart + ForwardVector*RayLength;
		return true;
	}
	return false;
}

bool URayInput::GetHitResult(const FVector& RayStart, const FVector& RayEnd, const TArray<AActor*>& OptionalListOfIgnoredActors, FHitResult& Hit)
{
	const bool bTraceComplex = true;
	FCollisionQueryParams TraceParams(NAME_None, bTraceComplex, nullptr);

	if (OptionalListOfIgnoredActors.Num() > 0)
	{
		TraceParams.AddIgnoredActors(OptionalListOfIgnoredActors);
		//UE_LOG(LogRayInput, Log, TEXT("Ingores %d,%s"), OptionalListOfIgnoredActors.Num(), *(OptionalListOfIgnoredActors[0]->GetName()));
	}

	//UE_LOG(LogRayInput, Log, TEXT("Ingores %d"), OptionalListOfIgnoredActors.Num());

	FCollisionObjectQueryParams EverythingButGizmos(FCollisionObjectQueryParams::AllObjects);
	EverythingButGizmos.RemoveObjectTypesToQuery(COLLISION_GIZMO);
	return GetWorld()->LineTraceSingleByObjectType(Hit, RayStart, RayEnd, EverythingButGizmos, TraceParams);
}

void URayInput::SetCaster(USceneComponent* CasterComponent)
{
	this->Caster = CasterComponent;
}

void URayInput::AddIgnoreActor(AActor* Actor)
{
	if (Actor != nullptr && Actor->IsValidLowLevel())
	{
		DefaultIgnores.Push(Actor);
	}
}

void URayInput::ClearIgnores()
{
	DefaultIgnores.Empty();
}