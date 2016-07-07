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

protected:

};
