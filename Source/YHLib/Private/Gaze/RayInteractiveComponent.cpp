// Fill out your copyright notice in the Description page of Project Settings.

#include "RayInteractiveComponent.h"


// Sets default values for this component's properties
URayInteractiveComponent::URayInteractiveComponent()
	:bHaveHoverColor(false),
	HoverColor(FLinearColor::Blue),
	bShowDefaultHover(true)
{

}

void URayInteractiveComponent::RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput)
{
	OnRayEnter.Broadcast(HitLocation, HitComponent, Hit);
}

void URayInteractiveComponent::RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput)
{
	OnRayStay.Broadcast(HitLocation, HitComponent, Hit);
}

void URayInteractiveComponent::RayExit(UActorComponent* HitComponent, URayInput* RayInput)
{
	OnRayExit.Broadcast(HitComponent);
}

void URayInteractiveComponent::KeyDown(FKey Key, URayInput* RayInput, bool bRepeat)
{
	OnKeyDown.Broadcast(Key);
}

void URayInteractiveComponent::KeyUp(FKey Key, URayInput* RayInput)
{
	OnKeyUp.Broadcast(Key);
}

void URayInteractiveComponent::KeyDownEvent(FKeyEvent KeyEvent, URayInput* RayInput)
{
	OnKeyDown.Broadcast(KeyEvent.GetKey());
}

void URayInteractiveComponent::KeyUpEvent(FKeyEvent KeyEvent, URayInput* RayInput)
{
	OnKeyUp.Broadcast(KeyEvent.GetKey());
}

void URayInteractiveComponent::PressPointerKey(FKey Key, URayInput* RayInput)
{
	OnPressPointer.Broadcast(Key);
}

void URayInteractiveComponent::ProcessKeyChar(const FString& Characters, URayInput* RayInput,bool bRepeat)
{

}

void URayInteractiveComponent::ReleasePointerKey(FKey Key, URayInput* RayInput)
{
	OnReleasePointer.Broadcast(Key);
}

bool URayInteractiveComponent::IsHover()
{
	return true;
}

bool URayInteractiveComponent::IsHoverChanged()
{
	return false;
}

bool URayInteractiveComponent::IsProtrudeThrough()
{
	return true;
}
