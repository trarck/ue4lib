// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazeInput.h"


// Sets default values
UGazeInput::UGazeInput()
{
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}

void UGazeInput::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UGazeInput::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UGazeInput::GetHitResultFromRay(FVector Start, FVector End, TArray<AActor*>* OptionalListOfIgnoredActors, FHitResult& Hit)
{

	const bool bTraceComplex = true;
	FCollisionQueryParams TraceParams(NAME_None, bTraceComplex, nullptr);

	if (OptionalListOfIgnoredActors != nullptr)
	{
		TraceParams.AddIgnoredActors(*OptionalListOfIgnoredActors);
	}

	FCollisionObjectQueryParams EverythingButGizmos(FCollisionObjectQueryParams::AllObjects);
	EverythingButGizmos.RemoveObjectTypesToQuery(COLLISION_GIZMO);
	return GetWorld()->LineTraceSingleByObjectType(Hit, Start, End, EverythingButGizmos, TraceParams);
}
