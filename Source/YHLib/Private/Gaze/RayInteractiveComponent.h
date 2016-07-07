// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInteractiveComponent.generated.h"


UCLASS(ClassGroup = (Gaze), meta = (BlueprintSpawnableComponent))
class URayInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URayInteractiveComponent();

	virtual void OnRayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnRayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void OnRayExit(UActorComponent* HitComponent);

	//if actor have special logic check hover like umg.
	virtual bool IsHover();

	//from sub target to another target.if no sub always return true
	virtual bool IsHoverChanged();

	virtual bool IsProtrudeThrough();

	FORCEINLINE bool HaveHoverColor() const
	{
		return bHaveHoverColor;
	}

	FORCEINLINE const FLinearColor& GetHoverColor()
	{
		return HoverColor;
	}

protected:
	/** response the ray input  should change laser color */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	bool bHaveHoverColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	FLinearColor HoverColor;
};
