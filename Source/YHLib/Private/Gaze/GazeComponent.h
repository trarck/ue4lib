// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGazeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGazeComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	virtual void OnGazeEnter(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnGazeStay(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnGazeExit(FVector HitLocation, UActorComponent* HitComponent);
	
};
