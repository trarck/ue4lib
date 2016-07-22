// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "JumpNavAIController.h"
#include "Navigation/JumpPathFollowingComponent.h"

AJumpNavAIController::AJumpNavAIController(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer.SetDefaultSubobjectClass<UJumpPathFollowingComponent>(TEXT("PathFollowingComponent")))
{

}


