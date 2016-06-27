// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "TestFindComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogTestFind, Log, All);

// Sets default values for this component's properties
UTestFindComponent::UTestFindComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;
	UE_LOG(LogTestFind, Log, TEXT("UTestFindComponent::Construct"));
	// ...
}

int UTestFindComponent::FindSubComponent()
{
	UTestSubComponent* Sub= GetOwner()->FindComponentByClass<UTestSubComponent>();
	if(Sub)
	{
		UE_LOG(LogTestFind, Log, TEXT("UTestFindComponent::FindSubComponent ok"));
	}else{
		UE_LOG(LogTestFind, Log, TEXT("UTestFindComponent::FindSubComponent fail"));
	}
	return 1;
}