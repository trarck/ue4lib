// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInteractiveComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRayEnterSignature, const FVector&, HitLocation, UActorComponent* ,HitComponent, const FHitResult&,Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRayStaySignature, const FVector&, HitLocation, UActorComponent*, HitComponent, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayExitSignature,UActorComponent*, HitComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayKeyDownSignature, FKey, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayKeyUpSignature, FKey, Key);

UCLASS(ClassGroup = (Gaze), meta = (BlueprintSpawnableComponent))
class URayInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URayInteractiveComponent();

	virtual void RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit);

	virtual void RayExit(UActorComponent* HitComponent);

	virtual void KeyDown(FKey Key);

	virtual void KeyUp(FKey Key);	

	virtual void KeyDownEvent(FKeyEvent KeyEvent);

	virtual void KeyUpEvent(FKeyEvent KeyEvent);

	//if actor have special logic check hover like umg.
	virtual bool IsHover();

	//from sub target to another target.if no sub always return true
	virtual bool IsHoverChanged();

	virtual bool IsProtrudeThrough();

	virtual bool HaveHoverColor() const
	{
		return bHaveHoverColor;
	}

	virtual const FLinearColor& GetHoverColor()
	{
		return HoverColor;
	}

	virtual bool IsShowDefaultHover() const
	{
		return bShowDefaultHover;
	}

public://event
	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayEnterSignature OnRayEnter;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayStaySignature OnRayStay;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayExitSignature OnRayExit;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayKeyDownSignature OnKeyDown;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayKeyUpSignature OnKeyUp;

protected:

	/** response the ray input  should change laser color */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	bool bHaveHoverColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	FLinearColor HoverColor;

	//show hover animation by hover,no default hover animation
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	bool bShowDefaultHover;
};
