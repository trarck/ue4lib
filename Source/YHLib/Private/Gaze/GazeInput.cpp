// Fill out your copyright notice in the Description page of Project Settings.

#include "GazeInput.h"
#include "Engine.h"

UGazeInput::UGazeInput()
	:PlayerIndex(0)
{
}

void UGazeInput::BeginPlay()
{
	Super::BeginPlay();

	APawn* Player = Cast<APawn>(GetOwner());
	if (Player != nullptr)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Player->GetController());
		if (PlayerController != nullptr) {
			PlayerCameraManager = PlayerController->PlayerCameraManager;
		}
	}

	if (PlayerCameraManager == nullptr)
	{
		PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(this, PlayerIndex);
	}
}

bool UGazeInput::GetRayPointer(FVector& RayStart, FVector& RayEnd)
{
	if (PlayerCameraManager)
	{
		FVector ForwardVector = PlayerCameraManager->GetCameraRotation().Quaternion().GetForwardVector();
		RayStart = PlayerCameraManager->GetCameraLocation();
		RayEnd = RayStart + ForwardVector*RayLength;
		return true;
	}

	return false;
}
