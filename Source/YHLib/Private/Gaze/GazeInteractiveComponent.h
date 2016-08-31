// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RayInteractiveComponent.h"
#include "GazeInteractiveComponent.generated.h"

UCLASS( ClassGroup=(Gaze), meta=(BlueprintSpawnableComponent) )
class UGazeInteractiveComponent : public URayInteractiveComponent
{
	GENERATED_BODY()
public:	
		
	// Sets default values for this component's properties
	UGazeInteractiveComponent();

	virtual float GetActionDuration() const
	{
		return ActionDuration;
	}

protected:
	/** zero use global config*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float ActionDuration;
};
