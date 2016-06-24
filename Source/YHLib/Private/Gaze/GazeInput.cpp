// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazeInput.h"

bool UGazeInput::GetRayPointer(FVector& RayStart, FVector& RayEnd)
{
	APlayerCameraManager* PlayerCameraManager= UGameplayStatics::GetPlayerCameraManager(this, 0);

	if (PlayerCameraManager)
	{
		FVector ForwardVector = PlayerCameraManager->GetCameraRotation().Quaternion().GetForwardVector();
		RayStart = PlayerCameraManager->GetCameraLocation();
		RayEnd = RayStart + ForwardVector*RayLength;
		return true;
	}

	return false;
}
