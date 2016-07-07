// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazeActionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActionSignature);

UCLASS( ClassGroup=(Gaze), meta=(BlueprintSpawnableComponent) )
class UGazeActionComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
		
	// Sets default values for this component's properties
	UGazeActionComponent();

	UPROPERTY(BlueprintAssignable, Category = "Gaze")
	FActionSignature OnAction;

	float GetActionDuration() const
	{
		return ActionDuration;
	}

protected:

	/** zero use global config*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float ActionDuration;
};
