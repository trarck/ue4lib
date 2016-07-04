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

	virtual bool RestartHover();

	virtual bool Hoverable();

	UPROPERTY(BlueprintAssignable, Category = "Gaze")
	FActionSignature OnAction;

	float GetActionDuration() const
	{
		return ActionDuration;
	}

	bool HaveGazeColor() const
	{
		return bHaveGazeColor;
	}

	const FLinearColor& GetGazeColor()
	{
		return GazeColor;
	}

protected:

	/** zero use global config*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float ActionDuration;

	/** when gaze this actor shou show special color */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	bool bHaveGazeColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	FLinearColor GazeColor;
};
