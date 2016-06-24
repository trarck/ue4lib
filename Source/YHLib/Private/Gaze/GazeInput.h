// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gaze/RayInput.h"
#include "GazeInput.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UGazeInput : public URayInput
{
	GENERATED_BODY()

public:

	virtual bool GetRayPointer(FVector& RayStart, FVector& RayEnd) override;

};
