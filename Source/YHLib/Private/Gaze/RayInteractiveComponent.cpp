// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "RayInteractiveComponent.h"


// Sets default values for this component's properties
URayInteractiveComponent::URayInteractiveComponent()
	:bHaveHoverColor(false),
	HoverColor(FLinearColor::Blue)
{

}

void URayInteractiveComponent::OnRayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{

}

void URayInteractiveComponent::OnRayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{

}

void URayInteractiveComponent::OnRayExit(UActorComponent* HitComponent)
{

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