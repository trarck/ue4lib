// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "TestOverrideComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogTestOverride, Log, All);

// Sets default values for this component's properties
UTestOverrideComponent::UTestOverrideComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
	UE_LOG(LogTestOverride, Log, TEXT("TestOverrideComponent::Construct"));
	// ...
}

int UTestOverrideComponent::OverFuna()
{
	FVector Vec;
	OverFunb(Vec);
	UE_LOG(LogTestOverride, Log, TEXT("vector:%f,%f,%f"), Vec.X, Vec.Y, Vec.Z);
	return 1;
}

void UTestOverrideComponent::OverFunb_Implementation(FVector& Vec)
{
	Vec = FVector(1.0f, 1.0f, 1.0f);
}