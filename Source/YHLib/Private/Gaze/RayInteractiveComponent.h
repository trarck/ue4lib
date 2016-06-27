// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInteractiveComponent.generated.h"


UCLASS( ClassGroup=(Gaze), meta=(BlueprintSpawnableComponent) )
class URayInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URayInteractiveComponent();

	virtual void OnRayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnRayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnRayExit(UActorComponent* HitComponent);	
};
