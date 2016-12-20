// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetInteractionManagerComponent.h"

#include "WidgetComponent.h"
#include "Gaze/GazeDefine.h"
#include "Gaze/RayInput.h"

DEFINE_LOG_CATEGORY_STATIC(LogWidgetInteractionManagerComponent, Log, All);

// Sets default values for this component's properties
UWidgetInteractionManagerComponent::UWidgetInteractionManagerComponent()
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

void UWidgetInteractionManagerComponent::RegisterProcess(class URayInput* InRayInput)
{
	//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("RegisterProcess"));
	//remove listener from old
	if (RayInput && RayInput->IsValidLowLevel())
	{
		//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("RegisterProcess remove old"));
		RayInput->OnProcessRay.RemoveDynamic(this, &UWidgetInteractionManagerComponent::ProcessRayHit);
		RayInput->OnPressKey.RemoveDynamic(this, &UWidgetInteractionManagerComponent::OnKeyDown);
		RayInput->OnReleaseKey.RemoveDynamic(this, &UWidgetInteractionManagerComponent::OnKeyUp);
		RayInput->OnSendKeyChar.RemoveDynamic(this, &UWidgetInteractionManagerComponent::OnProcessKeyChar);
		RayInput->OnPressPointerKey.RemoveDynamic(this, &UWidgetInteractionManagerComponent::OnPressPointerKey);
		RayInput->OnReleasePointerKey.RemoveDynamic(this, &UWidgetInteractionManagerComponent::OnReleasePointerKey);
	}

	RayInput = InRayInput;

	//register RayInput event 
	if (RayInput)
	{
		//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("RegisterProcess register new"));
		RayInput->OnProcessRay.AddDynamic(this, &UWidgetInteractionManagerComponent::ProcessRayHit);
		RayInput->OnPressKey.AddDynamic(this, &UWidgetInteractionManagerComponent::OnKeyDown);
		RayInput->OnReleaseKey.AddDynamic(this, &UWidgetInteractionManagerComponent::OnKeyUp);
		RayInput->OnSendKeyChar.AddDynamic(this, &UWidgetInteractionManagerComponent::OnProcessKeyChar);
		RayInput->OnPressPointerKey.AddDynamic(this, &UWidgetInteractionManagerComponent::OnPressPointerKey);
		RayInput->OnReleasePointerKey.AddDynamic(this, &UWidgetInteractionManagerComponent::OnReleasePointerKey);
	}
}

void UWidgetInteractionManagerComponent::ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay)
{
#if USE_NEW_INPUT_SYSTEM
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
			LocalHitLocation = HoveredWidgetComponent->GetLastLocalHitLocation();
		}
	}

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
		//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("move"));
		check(HoveredWidgetComponent);
		LastWigetPath = WidgetPathUnderFinger;

		FSlateApplication::Get().RoutePointerMoveEvent(WidgetPathUnderFinger, PointerEvent, false);
	}
	else
	{
		//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("empty"));
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().RoutePointerMoveEvent(EmptyWidgetPath, PointerEvent, false);

		LastWigetPath = FWeakWidgetPath();
	}

	if (HoveredWidgetComponent)
	{
		HoveredWidgetComponent->RequestRedraw();
	}
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
		if (OldHoveredWidget)
		{
			OldHoveredWidget->RequestRedraw();
		}
		bWidgetChange = true;
	}
#endif
}

void UWidgetInteractionManagerComponent::OnKeyDown(const FKey& Key, bool bRepeat)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), bRepeat, KeyCode, CharCode);
	UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("[%llu]OnKeyDown before %s:KeyCode:%d,CharCode:%d"), GFrameCounter, *Key.GetDisplayName().ToString(), KeyCode, CharCode);
	bool DownResult = FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);
	UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("[%llu]OnKeyDown after %s:KeyCode:%d,CharCode:%d"), GFrameCounter, *Key.GetDisplayName().ToString(), KeyCode, CharCode);
	if (CharCodePtr)
	{
		FCharacterEvent CharacterEvent(CharCode, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
}

void UWidgetInteractionManagerComponent::OnKeyUp(const FKey& Key)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), false, KeyCode, CharCode);
	FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
}

void UWidgetInteractionManagerComponent::OnProcessKeyChar(const FString& Characters, bool bRepeat)
{
	bool bProcessResult = false;

	for (int32 CharIndex = 0; CharIndex < Characters.Len(); CharIndex++)
	{
		TCHAR CharKey = Characters[CharIndex];

		FCharacterEvent CharacterEvent(CharKey, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		bProcessResult |= FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
}

void UWidgetInteractionManagerComponent::OnPressPointerKey(const FKey& Key)
{
#if USE_NEW_INPUT_SYSTEM
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
	UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("[%llu]PressPointerKey before %s:userindex:%d"), GFrameCounter, *Key.GetDisplayName().ToString(), PointerEvent.GetUserIndex());

	FReply Reply = FSlateApplication::Get().RoutePointerDownEvent(WidgetPathUnderFinger, PointerEvent);
	//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("[%llu]PressPointerKey after %s"), GFrameCounter, *Key.GetDisplayName().ToString());
#endif
}

void UWidgetInteractionManagerComponent::OnReleasePointerKey(const FKey& Key)
{
#if USE_NEW_INPUT_SYSTEM
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
	//UE_LOG(LogWidgetInteractionManagerComponent, Log, TEXT("[%llu]ReleasePointerKey before %s"), GFrameCounter,*Key.GetDisplayName().ToString());
	//	ActiveWidgetAndPointer.Widget->OnMouseButtonUp(ActiveWidgetAndPointer.Geometry, KeyEvent);
	FSlateApplication::Get().RoutePointerUpEvent(WidgetPathUnderFinger, PointerEvent);
#endif
}

bool UWidgetInteractionManagerComponent::IsHoverChanged()
{
	return bWidgetChange;
}

UWidgetComponent* UWidgetInteractionManagerComponent::GetHoveredWidgetComponent() const
{
	return HoveredWidgetComponent;
}

bool UWidgetInteractionManagerComponent::IsOverInteractableWidget() const
{
	return bIsHoveredWidgetInteractable;
}

bool UWidgetInteractionManagerComponent::IsOverFocusableWidget() const
{
	return bIsHoveredWidgetFocusable;
}

bool UWidgetInteractionManagerComponent::IsOverHitTestVisibleWidget() const
{
	return bIsHoveredWidgetHitTestVisible;
}

const FWeakWidgetPath& UWidgetInteractionManagerComponent::GetHoveredWidgetPath() const
{
	return LastWigetPath;
}