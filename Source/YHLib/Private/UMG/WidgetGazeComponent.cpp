// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetGazeComponent.h"

#include "WidgetComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidgetGaze, Log, All);

// Sets default values for this component's properties
UWidgetGazeComponent::UWidgetGazeComponent()
	:bWidgetHover(false),
	bWidgetChange(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UWidgetGazeComponent::RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayEndter"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);
	
	TArray<FWidgetAndPointer> widgets = WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false);

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(widgets);
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

	if (widgets.Num() > 0)
	{
		ActiveWidgetAndPointer = widgets.Last();
		bWidgetHover = IsWidgetInteractive(ActiveWidgetAndPointer.Widget);
	}
}

void UWidgetGazeComponent::RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayStay"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);
	TArray<FWidgetAndPointer> widgets = WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false);
	FWidgetPath WidgetPathUnderFinger = FWidgetPath(widgets);
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
		if (widgets.Num() > 0)
		{
			if (!(ActiveWidgetAndPointer == widgets.Last()))
			{
				ActiveWidgetAndPointer = widgets.Last();	
				bWidgetChange = true;
			}
			else
			{
				bWidgetChange = false;
			}

			bWidgetHover = IsWidgetInteractive(ActiveWidgetAndPointer.Widget);
		}
	}
}

void UWidgetGazeComponent::RayExit(UActorComponent* HitComponent)
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

void UWidgetGazeComponent::KeyDown(FKey Key)
{
	Super::KeyDown(Key);
	if(ActiveWidgetAndPointer)
}

void UWidgetGazeComponent::KeyUp(FKey Key)
{
	Super::KeyUp(Key);
}

bool UWidgetGazeComponent::IsHover()
{
	return bWidgetHover;
}

bool UWidgetGazeComponent::IsHoverChanged()
{
	return bWidgetChange;
}

bool UWidgetGazeComponent::IsWidgetInteractive(TSharedRef<SWidget> Widget)
{
	return Widget->IsInteractable();
}