// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetInputManagerComponent.h"

#include "WidgetComponent.h"

#include "Gaze/GazeDefine.h"
#include "Gaze/RayInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidgetInputManagerComponent, Log, All);


#if !USE_NEW_INPUT_SYSTEM
struct FGazePointerEvent : public FPointerEvent
{
public:
	FGazePointerEvent(
		uint32 InUserIndex,
		uint32 InPointerIndex,
		const FVector2D& InScreenSpacePosition,
		const FVector2D& InLastScreenSpacePosition,
		const TSet<FKey>& InPressedButtons,
		FKey InEffectingButton,
		float InWheelDelta,
		const FModifierKeysState& InModifierKeys
	)
		: FPointerEvent(InPointerIndex, InScreenSpacePosition, InLastScreenSpacePosition, InPressedButtons, InEffectingButton, InWheelDelta, InModifierKeys)
	{
		UserIndex = InUserIndex;
	}
};
#endif
// Sets default values for this component's properties
UWidgetInputManagerComponent::UWidgetInputManagerComponent()
	:bWidgetChange(false),
	bIsHoveredWidgetInteractable(false),
	bIsHoveredWidgetFocusable(false),
	bIsHoveredWidgetHitTestVisible(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UWidgetInputManagerComponent::RegisterProcess(class URayInput* InRayInput)
{
	//UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("RegisterProcess"));
	//remove listener from old
	if (RayInput && RayInput->IsValidLowLevel())
	{
		//UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("RegisterProcess remove old"));
		RayInput->OnProcessRay.RemoveDynamic(this, &UWidgetInputManagerComponent::ProcessRayHit);
		RayInput->OnPressKey.RemoveDynamic(this, &UWidgetInputManagerComponent::OnKeyDown);
		RayInput->OnReleaseKey.RemoveDynamic(this, &UWidgetInputManagerComponent::OnKeyUp);
		RayInput->OnSendKeyChar.RemoveDynamic(this, &UWidgetInputManagerComponent::OnProcessKeyChar);
		RayInput->OnPressPointerKey.RemoveDynamic(this, &UWidgetInputManagerComponent::OnPressPointerKey);
		RayInput->OnReleasePointerKey.RemoveDynamic(this, &UWidgetInputManagerComponent::OnReleasePointerKey);
	}

	RayInput = InRayInput;

	//register RayInput event 
	if (RayInput)
	{
		//UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("RegisterProcess register new"));
		RayInput->OnProcessRay.AddDynamic(this, &UWidgetInputManagerComponent::ProcessRayHit);
		RayInput->OnPressKey.AddDynamic(this, &UWidgetInputManagerComponent::OnKeyDown);
		RayInput->OnReleaseKey.AddDynamic(this, &UWidgetInputManagerComponent::OnKeyUp);
		RayInput->OnSendKeyChar.AddDynamic(this, &UWidgetInputManagerComponent::OnProcessKeyChar);
		RayInput->OnPressPointerKey.AddDynamic(this, &UWidgetInputManagerComponent::OnPressPointerKey);
		RayInput->OnReleasePointerKey.AddDynamic(this, &UWidgetInputManagerComponent::OnReleasePointerKey);
	}
}

void UWidgetInputManagerComponent::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay)
{
	bIsHoveredWidgetInteractable = false;
	bIsHoveredWidgetFocusable = false;
	bIsHoveredWidgetHitTestVisible = false;
	bWidgetChange = false;

	LocalHitLocation = FVector2D(0, 0);
	FWidgetPath WidgetPathUnderFinger;


	UWidgetComponent* OldHoveredWidget = HoveredWidgetComponent;

	HoveredWidgetComponent = nullptr;

	if (bHit)
	{
		HoveredWidgetComponent = Cast<UWidgetComponent>(HitResult.GetComponent());
		if (HoveredWidgetComponent)
		{
			WidgetPathUnderFinger = FWidgetPath(HoveredWidgetComponent->GetHitWidgetPath(HitResult.ImpactPoint, /*bIgnoreEnabledStatus*/ false));
#if USE_NEW_INPUT_SYSTEM
			LocalHitLocation = HoveredWidgetComponent->GetLastLocalHitLocation();
#else
			HoveredWidgetComponent->GetLocalHitLocation(HitResult.ImpactPoint, LocalHitLocation);
#endif	//USE_NEW_INPUT_SYSTEM
		}
	}

#if USE_NEW_INPUT_SYSTEM
	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);
#else
	FGazePointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);
#endif //USE_NEW_INPUT_SYSTEM
	if (WidgetPathUnderFinger.IsValid())
	{
		//UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("move"));
		check(HoveredWidgetComponent);
		LastWigetPath = WidgetPathUnderFinger;

		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);
	}
	else
	{
		//UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("empty"));
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);

		LastWigetPath = FWeakWidgetPath();
	}
#if USE_NEW_INPUT_SYSTEM
	if (HoveredWidgetComponent)
	{
		HoveredWidgetComponent->RequestRedraw();
	}
#endif //USE_NEW_INPUT_SYSTEM
	LastLocalHitLocation = LocalHitLocation;

	if (WidgetPathUnderFinger.IsValid())
	{
		const FArrangedChildren::FArrangedWidgetArray& AllArrangedWidgets = WidgetPathUnderFinger.Widgets.GetInternalArray();
		for (const FArrangedWidget& ArrangedWidget : AllArrangedWidgets)
		{
			const TSharedRef<SWidget>& Widget = ArrangedWidget.Widget;
			if (Widget->IsInteractable())
			{
				bIsHoveredWidgetInteractable = true;
			}

			if (Widget->SupportsKeyboardFocus())
			{
				bIsHoveredWidgetFocusable = true;
			}

			if (Widget->GetVisibility().IsHitTestVisible())
			{
				bIsHoveredWidgetHitTestVisible = true;
			}
		}
	}

	if (HoveredWidgetComponent != OldHoveredWidget)
	{
#if USE_NEW_INPUT_SYSTEM
		if (OldHoveredWidget)
		{
			OldHoveredWidget->RequestRedraw();
		}
#endif //USE_NEW_INPUT_SYSTEM
		bWidgetChange = true;
	}
}

void UWidgetInputManagerComponent::OnKeyDown(const FKey& Key, bool bRepeat)
{
	if (!LastWigetPath.IsValid())
	{
		return;
	}

	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), bRepeat, KeyCode, CharCode);
	UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("[%llu]OnKeyDown before %s,KeyCode:%d,CharCode:%d"), GFrameCounter, *Key.GetDisplayName().ToString(), KeyCode, CharCode);

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
	FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets.Last();
	WidgetAndPointer.Widget->OnKeyDown(WidgetAndPointer.Geometry, KeyEvent);
} 

void UWidgetInputManagerComponent::OnKeyUp(const FKey& Key)
{
	if (!LastWigetPath.IsValid())
	{
		return;
	}

	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), false, KeyCode, CharCode);
	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
	FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets.Last();
	WidgetAndPointer.Widget->OnKeyUp(WidgetAndPointer.Geometry, KeyEvent);
}

void UWidgetInputManagerComponent::OnProcessKeyChar(const FString& Characters, bool bRepeat)
{

}

void UWidgetInputManagerComponent::OnPressPointerKey(const FKey& Key)
{
	if (PressedKeys.Contains(Key))
	{
		return;
	}

	PressedKeys.Add(Key);

	if (!LastWigetPath.IsValid())
	{
		return;
	}

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
	PointerKeyWigetPath = LastWigetPath;

#if USE_NEW_INPUT_SYSTEM
	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
#else
	FGazePointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);
#endif //USE_NEW_INPUT_SYSTEM
	UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("[%llu]PressPointerKey before %s,userIndex:%d"), GFrameCounter,*Key.GetDisplayName().ToString(), PointerEvent.GetUserIndex());
	FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets.Last();
	WidgetAndPointer.Widget->OnMouseButtonDown(WidgetAndPointer.Geometry, PointerEvent);
}

void UWidgetInputManagerComponent::OnReleasePointerKey(const FKey& Key)
{
	if (!PressedKeys.Contains(Key))
	{
		return;
	}
	PressedKeys.Remove(Key);

	if (!PointerKeyWigetPath.IsValid())
	{
		return;
	}

	FWidgetPath WidgetPathUnderFinger = PointerKeyWigetPath.ToWidgetPath();
#if USE_NEW_INPUT_SYSTEM
	FPointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
#else
	FGazePointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		FKey(),
		0.0f,
		ModifierKeys);
#endif
	UE_LOG(LogWidgetInputManagerComponent, Log, TEXT("[%llu]ReleasePointerKey before %s"), GFrameCounter,*Key.GetDisplayName().ToString());
	FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets.Last();
	WidgetAndPointer.Widget->OnMouseButtonUp(WidgetAndPointer.Geometry, PointerEvent);
}

bool UWidgetInputManagerComponent::IsHoverChanged()
{
	return bWidgetChange;
}

UWidgetComponent* UWidgetInputManagerComponent::GetHoveredWidgetComponent() const
{
	return HoveredWidgetComponent;
}

bool UWidgetInputManagerComponent::IsOverInteractableWidget() const
{
	return bIsHoveredWidgetInteractable;
}

bool UWidgetInputManagerComponent::IsOverFocusableWidget() const
{
	return bIsHoveredWidgetFocusable;
}

bool UWidgetInputManagerComponent::IsOverHitTestVisibleWidget() const
{
	return bIsHoveredWidgetHitTestVisible;
}

const FWeakWidgetPath& UWidgetInputManagerComponent::GetHoveredWidgetPath() const
{
	return LastWigetPath;
}