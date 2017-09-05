// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCallOrderComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogCallOrder, Log, All);

// Sets default values for this component's properties
UTestCallOrderComponent::UTestCallOrderComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	sign=false;

	UE_LOG(LogCallOrder, Log, TEXT("Constructor %s"),GetOwner()!=nullptr?TEXT("true"):TEXT("false"));
}

void UTestCallOrderComponent::MethadA()
{
	UE_LOG(LogCallOrder, Log,TEXT("MethadA %s"),GetOwner()!=nullptr?TEXT("true"):TEXT("false"));
}

void UTestCallOrderComponent::MethadB()
{
	UE_LOG(LogCallOrder, Log,TEXT("MethadB %s"),GetOwner()!=nullptr?TEXT("true"):TEXT("false"));
}

// Called when the game starts
void UTestCallOrderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogCallOrder, Log,TEXT("BeginPlay %s"),GetOwner()!=nullptr?TEXT("true"):TEXT("false"));
}


// Called every frame
void UTestCallOrderComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if(!sign)
	{
		UE_LOG(LogCallOrder, Log,TEXT("TickComponent"));
		sign=true;
	}	
}

