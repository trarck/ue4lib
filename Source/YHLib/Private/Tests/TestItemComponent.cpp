// Fill out your copyright notice in the Description page of Project Settings.

#include "TestItemComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogTestItem, Log, All);

// Sets default values for this component's properties
UTestItemComponent::UTestItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	UE_LOG(LogTestItem, Log, TEXT("UTestItemComponent::Construct"));
	// ...
}

UTestItemComponent::~UTestItemComponent()
{
    UE_LOG(LogTestItem, Log, TEXT("UTestItemComponent::Destroy[%s]"), *(MyName.ToString()));
}

// Called when the game starts
void UTestItemComponent::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTestItem, Log, TEXT("UTestItemComponent::BeginPlay"));
	// ...
	
}


// Called every frame
void UTestItemComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UTestItemComponent::BeginDestroy()
{
	Super::BeginDestroy();
	UE_LOG(LogTestItem, Log, TEXT("UTestItemComponent::BeginDestroy[%s]"), *(MyName.ToString()));
}

void UTestItemComponent::FinishDestroy()
{
	Super::FinishDestroy();
	UE_LOG(LogTestItem, Log, TEXT("UTestItemComponent::FinishDestroy[%s]"), *(MyName.ToString()));
}