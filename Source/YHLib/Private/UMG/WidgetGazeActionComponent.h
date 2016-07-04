// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gaze/GazeActionComponent.h"
#include "WidgetGazeActionComponent.generated.h"

UCLASS( ClassGroup=(Gaze), meta=(BlueprintSpawnableComponent) )
class UWidgetGazeActionComponent : public UGazeActionComponent
{
	GENERATED_BODY()
public:	
		
	// Sets default values for this component's properties
	UWidgetGazeActionComponent();

	virtual bool RestartHover() override;

	virtual bool Hoverable() override;

	UFUNCTION(BlueprintCallable,Category="Gaze")
	void SetWidgetHover(bool Value)
	{
		bWidgetHover = Value;
	}

	UFUNCTION(BlueprintCallable, Category = "Gaze")
		void SetWidgetHoverable(bool Value)
	{
		bWidgetHoverable = Value;
	}
protected:

	bool bWidgetHover;

	bool bWidgetHoverable;
};
