// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetGazeComponent.h"

#include "WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidgetGaze, Log, All);

// Sets default values for this component's properties
UWidgetGazeComponent::UWidgetGazeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UWidgetGazeComponent::OnRayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayEndter"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false));
	if (WidgetPathUnderFinger.IsValid())
	{
		FVector2D LastLocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

		FVector2D LocalHitLocation;
		WidgetComponent->GetLocalHitLocation(HitLocation, LocalHitLocation);

		TSet<FKey> PressedButtons;
		FPointerEvent PointerEvent(
			1,
			LocalHitLocation,
			LastLocalHitLocation,
			LocalHitLocation - LastLocalHitLocation,
			PressedButtons,
			FModifierKeysState());

		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);
	}
}

void UWidgetGazeComponent::OnRayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayStay"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false));
	if (WidgetPathUnderFinger.IsValid())
	{
		FVector2D LastLocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

		FVector2D LocalHitLocation;
		WidgetComponent->GetLocalHitLocation(HitLocation, LocalHitLocation);

		TSet<FKey> PressedButtons;
		FPointerEvent PointerEvent(
			1,
			LocalHitLocation,
			LastLocalHitLocation,
			LocalHitLocation - LastLocalHitLocation,
			PressedButtons,
			FModifierKeysState());

		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);
	}
}

void UWidgetGazeComponent::OnRayExit(UActorComponent* HitComponent)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayExit"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);
	
	FVector2D LastLocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

	TSet<FKey> PressedButtons;

	FPointerEvent PointerEvent(
		1,
		LastLocalHitLocation,
		LastLocalHitLocation,
		FVector2D::ZeroVector,
		PressedButtons,
		FModifierKeysState());

	FSlateApplication::Get().RoutePointerMoveEvent(FWidgetPath(), PointerEvent, false);
	
}