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
	UGazeInput();

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Gaze")
	int32 PlayerIndex;

	virtual void BeginPlay() override;

	/** use current player camera */
	virtual bool GetRayPointer(FVector& RayStart, FVector& RayEnd) override;

protected:

	UPROPERTY()
	APlayerCameraManager* PlayerCameraManager;
};
