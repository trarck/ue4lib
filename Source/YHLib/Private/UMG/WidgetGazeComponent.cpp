// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetGazeComponent.h"

#include "WidgetComponent.h"
#include "Gaze/RayInput.h"

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


void UWidgetGazeComponent::RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayEndter"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);

	TArray<FWidgetAndPointer> widgets = WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false);
	
	LocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(widgets);

	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);

	if (WidgetPathUnderFinger.IsValid())
	{
		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);

		LastWigetPath = WidgetPathUnderFinger;
	}
	else
	{
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);
		LastWigetPath = FWeakWidgetPath();
	}

	LastLocalHitLocation = LocalHitLocation;

	if (widgets.Num() > 0)
	{
		ActiveWidgetAndPointer = widgets.Last();
		bWidgetHover = IsWidgetInteractive(ActiveWidgetAndPointer.Widget);
	}
}

void UWidgetGazeComponent::RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayStay"));
	UWidgetComponent* WidgetComponent = Cast<UWidgetComponent>(HitComponent);
	TArray<FWidgetAndPointer> widgets = WidgetComponent->GetHitWidgetPath(HitLocation, /*bIgnoreEnabledStatus*/ false);
	LocalHitLocation = WidgetComponent->GetLastLocalHitLocation();

	FWidgetPath WidgetPathUnderFinger = FWidgetPath(widgets);

	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);

	if (WidgetPathUnderFinger.IsValid())
	{
		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);
		LastWigetPath = WidgetPathUnderFinger;
	}
	else
	{
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);
		LastWigetPath = FWeakWidgetPath();
	}

	LastLocalHitLocation = LocalHitLocation;

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

void UWidgetGazeComponent::RayExit(UActorComponent* HitComponent, URayInput* RayInput)
{
	//UE_LOG(LogWidgetGaze, Log, TEXT("OnRayExit"));

	LocalHitLocation = FVector2D(0, 0);

	FWidgetPath EmptyWidgetPath;

	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);

	FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);

	LastWigetPath = FWeakWidgetPath();

	LastLocalHitLocation = LocalHitLocation;
}

void UWidgetGazeComponent::KeyDown(FKey Key, URayInput* RayInput, bool bRepeat)
{
	Super::KeyDown(Key, RayInput, bRepeat);

	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), bRepeat, KeyCode, CharCode);
	bool DownResult = FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);

	if (CharCodePtr)
	{
		FCharacterEvent CharacterEvent(CharCode, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
}

void UWidgetGazeComponent::KeyUp(FKey Key, URayInput* RayInput)
{
	Super::KeyUp(Key, RayInput);

	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), false, KeyCode, CharCode);
	FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

void UWidgetGazeComponent::KeyDownEvent(FKeyEvent KeyEvent, URayInput* RayInput)
{
	Super::KeyDownEvent(KeyEvent, RayInput);

	bool DownResult = FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);

	if (KeyEvent.GetCharacter())
	{
		FCharacterEvent CharacterEvent(KeyEvent.GetCharacter(), ModifierKeys, RayInput->GetUserIndex(), KeyEvent.IsRepeat());
		FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
}

void UWidgetGazeComponent::KeyUpEvent(FKeyEvent KeyEvent, URayInput* RayInput)
{
	Super::KeyUpEvent(KeyEvent, RayInput);
	FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

void UWidgetGazeComponent::ProcessKeyChar(const FString& Characters, URayInput* RayInput,bool bRepeat)
{
	bool bProcessResult = false;

	for (int32 CharIndex = 0; CharIndex < Characters.Len(); CharIndex++)
	{
		TCHAR CharKey = Characters[CharIndex];

		FCharacterEvent CharacterEvent(CharKey, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		bProcessResult |= FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
}

void UWidgetGazeComponent::PressPointerKey(FKey Key, URayInput* RayInput)
{
	Super::PressPointerKey(Key, RayInput);
	if (PressedKeys.Contains(Key))
	{
		return;
	}
	PressedKeys.Add(Key);

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();

	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
	//ActiveWidgetAndPointer.Widget->OnMouseButtonDown(ActiveWidgetAndPointer.Geometry, KeyEvent);
	//UE_LOG(LogWidgetGaze, Log, TEXT("[%llu]PressPointerKey before %s"), GFrameCounter,*Key.GetDisplayName().ToString());

	FReply Reply = FSlateApplication::Get().RoutePointerDownEvent(WidgetPathUnderFinger, PointerEvent);
	//UE_LOG(LogWidgetGaze, Log, TEXT("[%llu]PressPointerKey after %s"), GFrameCounter, *Key.GetDisplayName().ToString());

}

void UWidgetGazeComponent::ReleasePointerKey(FKey Key, URayInput* RayInput)
{
	Super::ReleasePointerKey(Key, RayInput);

	if (!PressedKeys.Contains(Key))
	{
		return;
	}
	PressedKeys.Remove(Key);

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();

	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
	//UE_LOG(LogWidgetGaze, Log, TEXT("[%llu]ReleasePointerKey before %s"), GFrameCounter,*Key.GetDisplayName().ToString());
//	ActiveWidgetAndPointer.Widget->OnMouseButtonUp(ActiveWidgetAndPointer.Geometry, KeyEvent);
	FReply Reply = FSlateApplication::Get().RoutePointerUpEvent(WidgetPathUnderFinger, PointerEvent);
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

const FWeakWidgetPath& UWidgetGazeComponent::GetHoveredWidgetPath() const
{
	return LastWigetPath;
}