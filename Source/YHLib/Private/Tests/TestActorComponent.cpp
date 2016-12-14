// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "TestActorComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogTestActor, Log, All);

// Sets default values for this component's properties
UTestActorComponent::UTestActorComponent()
	:PublicPropOne(0),
	PublicPropTwo(0),
	ProtecedPropOne(0),
	ProtecedPropTwo(0)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTestActor, Log, TEXT("TestActorComponent::Construct"));
	// ...
}


// Called when the game starts
void UTestActorComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTestActor, Log, TEXT("TestActorComponent::BeginPlay"));
	// ...
	
}


// Called every frame
void UTestActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}


void UTestActorComponent::PubFuna()
{

}

void UTestActorComponent::PubFunb()
{

}

void UTestActorComponent::ProFuna()
{

}

void UTestActorComponent::ProFunb()
{

}

int UTestActorComponent::OverFuna()
{
	FVector Vec;
	OverFunb(Vec);
	UE_LOG(LogTestActor, Log, TEXT("vector:%f,%f,%f\n%d"), Vec.X, Vec.Y, Vec.Z, PublicPropTwo);
	return 1;
}

void UTestActorComponent::OverFunb_Implementation(FVector& Vec)
{
	Vec = FVector(1.0f, 1.0f, 1.0f);
}