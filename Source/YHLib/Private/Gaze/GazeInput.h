// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazeInput.generated.h"

UCLASS()
class UGazeInput : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UGazeInput();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetHitResultFromRay(FVector Start,FVector End, TArray<AActor*>* OptionalListOfIgnoredActors, FHitResult& Hit);
	
};
