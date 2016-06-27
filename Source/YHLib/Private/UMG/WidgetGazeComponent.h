// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gaze/RayInteractiveComponent.h"
#include "WidgetGazeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWidgetGazeComponent : public URayInteractiveComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetGazeComponent();
	
	virtual void OnRayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void OnRayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void OnRayExit(UActorComponent* HitComponent) override;

protected:
};
