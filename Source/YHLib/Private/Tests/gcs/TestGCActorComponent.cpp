// Fill out your copyright notice in the Description page of Project Settings.

#include "TestGCActorComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogTestGC, Log, All);

// Sets default values for this component's properties
UTestGCActorComponent::UTestGCActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTestGC, Log, TEXT("UTestGCActorComponent::Construct"));
	// ...
}

UTestGCActorComponent::~UTestGCActorComponent()
{
    UE_LOG(LogTestGC, Log, TEXT("UTestGCActorComponent::Destroy[%s]"), *(MyName.ToString()));
}

// Called when the game starts
void UTestGCActorComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTestGC, Log, TEXT("UTestGCActorComponent::BeginPlay"));
	// ...
	
}


// Called every frame
void UTestGCActorComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UTestGCActorComponent::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTestGC, Log, TEXT("UTestGCActorComponent::BeginDestroy[%s]"), *(MyName.ToString()));
}

void UTestGCActorComponent::FinishDestroy()
{
	Super::FinishDestroy();
	UE_LOG(LogTestGC, Log, TEXT("UTestGCActorComponent::FinishDestroy[%s]"), *(MyName.ToString()));
}