// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInteractiveComponent.generated.h"

class URayInput;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRayEnterSignature, const FVector&, HitLocation, UActorComponent* ,HitComponent, const FHitResult&,Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FRayStaySignature, const FVector&, HitLocation, UActorComponent*, HitComponent, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayExitSignature,UActorComponent*, HitComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayKeyDownSignature, FKey, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayKeyUpSignature, FKey, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayPressPointerSignature, FKey, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRayReleasePointerSignature, FKey, Key);

UCLASS(ClassGroup = (Gaze), meta = (BlueprintSpawnableComponent))
class URayInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	URayInteractiveComponent();

	virtual void RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit,URayInput* RayInput);

	virtual void RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit,URayInput* RayInput);

	virtual void RayExit(UActorComponent* HitComponent, URayInput* RayInput);

	virtual void KeyDown(FKey Key, URayInput* RayInput, bool bRepeat = false);

	virtual void KeyUp(FKey Key, URayInput* RayInput);

	virtual void KeyDownEvent(FKeyEvent KeyEvent, URayInput* RayInput);

	virtual void KeyUpEvent(FKeyEvent KeyEvent, URayInput* RayInput);

	virtual void PressPointerKey(FKey Key, URayInput* RayInput);

	virtual void ReleasePointerKey(FKey Key, URayInput* RayInput);

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

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayPressPointerSignature OnPressPointer;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FRayReleasePointerSignature OnReleasePointer;

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
