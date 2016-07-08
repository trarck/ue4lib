// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "RayInteractiveComponent.h"


// Sets default values for this component's properties
URayInteractiveComponent::URayInteractiveComponent()
	:bHaveHoverColor(false),
	HoverColor(FLinearColor::Blue),
	bSelfHoverShow(false)
{

}

void URayInteractiveComponent::RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	OnRayEnter.Broadcast(HitLocation, HitComponent, Hit);
}

void URayInteractiveComponent::RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	OnRayStay.Broadcast(HitLocation, HitComponent, Hit);
}

void URayInteractiveComponent::RayExit(UActorComponent* HitComponent)
{
	OnRayExit.Broadcast(HitComponent);
}

void URayInteractiveComponent::KeyDown(FKey Key)
{
	OnKeyDown.Broadcast(Key);
}

void URayInteractiveComponent::KeyUp(FKey Key)
{
	OnKeyUp.Broadcast(Key);
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
