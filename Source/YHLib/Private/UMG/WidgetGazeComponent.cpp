// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetGazeComponent.h"

#include "WidgetComponent.h"

// Sets default values for this component's properties
UWidgetGazeComponent::UWidgetGazeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWidgetGazeComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UWidgetGazeComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
}

void UWidgetGazeComponent::OnGaze(FVector WorldLocation,UActorComponent* ActorComponent)
{
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(ActorComponent);

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(WidgetComponent->GetHitWidgetPath(WorldLocation, /*bIgnoreEnabledStatus*/ false));
	if (WidgetPathUnderFinger.IsValid())
	{
		FVector2D LastLocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

		FVector2D LocalHitLocation;
		WidgetComponent->GetLocalHitLocation(WorldLocation, LocalHitLocation);

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

void UWidgetGazeComponent::OnGazeEnter(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{

}

void UWidgetGazeComponent::OnGazeStay(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit)
{

}

void UWidgetGazeComponent::OnGazeExit(FVector HitLocation, UActorComponent* HitComponent)
{

}