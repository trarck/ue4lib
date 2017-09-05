// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetInteractionManagerComponent.h"

#include "WidgetComponent.h"
#include "Gaze/GazeDefine.h"
#include "Gaze/RayInput.h"
#include "GazePointerEvent.h"

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

void UWidgetInteractionManagerComponent::OnKeyDown(const FKey& Key, bool bRepeat)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), bRepeat, KeyCode, CharCode);
#if USE_NEW_INPUT_SYSTEM
	bool DownResult = FSlateApplication::Get().ProcessKeyDownEvent(KeyEvent);	
	if (CharCodePtr)
	{
		FCharacterEvent CharacterEvent(CharCode, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
#else
	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
	FReply Reply=FReply::Unhandled();
	for (int WidgetIndex = WidgetPathUnderFinger.Widgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
	{
		FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets[WidgetIndex];
		Reply = WidgetAndPointer.Widget->OnKeyDown(WidgetAndPointer.Geometry, KeyEvent);
		if (Reply.IsEventHandled())
		{
			break;
		}
	}
#endif //USE_NEW_INPUT_SYSTEM
}

void UWidgetInteractionManagerComponent::OnKeyUp(const FKey& Key)
{
	const uint32* KeyCodePtr;
	const uint32* CharCodePtr;
	FInputKeyManager::Get().GetCodesFromKey(Key, KeyCodePtr, CharCodePtr);

	uint32 KeyCode = KeyCodePtr ? *KeyCodePtr : 0;
	uint32 CharCode = CharCodePtr ? *CharCodePtr : 0;

	FKeyEvent KeyEvent(Key, ModifierKeys, RayInput->GetUserIndex(), false, KeyCode, CharCode);
#if USE_NEW_INPUT_SYSTEM	
	FSlateApplication::Get().ProcessKeyUpEvent(KeyEvent);
#else
	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();	
	FReply Reply=FReply::Unhandled();
	for (int WidgetIndex = WidgetPathUnderFinger.Widgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
	{
		FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets[WidgetIndex];
		Reply = WidgetAndPointer.Widget->OnKeyUp(WidgetAndPointer.Geometry, KeyEvent);
		if (Reply.IsEventHandled())
		{
			break;
		}
	}
#endif	
}

void UWidgetInteractionManagerComponent::OnProcessKeyChar(const FString& Characters, bool bRepeat)
{
#if USE_NEW_INPUT_SYSTEM
	bool bProcessResult = false;

	for (int32 CharIndex = 0; CharIndex < Characters.Len(); CharIndex++)
	{
		TCHAR CharKey = Characters[CharIndex];

		FCharacterEvent CharacterEvent(CharKey, ModifierKeys, RayInput->GetUserIndex(), bRepeat);
		bProcessResult |= FSlateApplication::Get().ProcessKeyCharEvent(CharacterEvent);
	}
#endif//USE_NEW_INPUT_SYSTEM
}

void UWidgetInteractionManagerComponent::OnPressPointerKey(const FKey& Key)
{

	if (PressedKeys.Contains(Key))
	{
		return;
	}
	PressedKeys.Add(Key);

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
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
	FReply Reply = FSlateApplication::Get().RoutePointerDownEvent(WidgetPathUnderFinger, PointerEvent);
#else
	FGazePointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
	FReply Reply=FReply::Unhandled();
	for (int WidgetIndex = WidgetPathUnderFinger.Widgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
	{
		FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets[WidgetIndex];
		Reply = WidgetAndPointer.Widget->OnMouseButtonDown(WidgetAndPointer.Geometry, PointerEvent);
		FSlateApplication::Get().ProcessReply(WidgetPathUnderFinger, Reply, nullptr, &PointerEvent, RayInput->GetUserIndex());
		if (Reply.IsEventHandled())
		{
			break;
		}
	}
#endif //USE_NEW_INPUT_SYSTEM
}

void UWidgetInteractionManagerComponent::OnReleasePointerKey(const FKey& Key)
{

	if (!PressedKeys.Contains(Key))
	{
		return;
	}
	PressedKeys.Remove(Key);

	FWidgetPath WidgetPathUnderFinger = LastWigetPath.ToWidgetPath();
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
	FSlateApplication::Get().RoutePointerUpEvent(WidgetPathUnderFinger, PointerEvent);
#else
	FGazePointerEvent PointerEvent(
		RayInput->GetUserIndex(),
		RayInput->PointerIndex,
		LocalHitLocation,
		LastLocalHitLocation,
		PressedKeys,
		Key,
		0.0f,
		ModifierKeys);
	FReply Reply = FReply::Unhandled();
	for (int WidgetIndex = WidgetPathUnderFinger.Widgets.Num() - 1; WidgetIndex >= 0; --WidgetIndex)
	{
		FArrangedWidget& WidgetAndPointer = WidgetPathUnderFinger.Widgets[WidgetIndex];
		Reply = WidgetAndPointer.Widget->OnMouseButtonUp(WidgetAndPointer.Geometry, PointerEvent);
		//使用空的WidgetPath,避免在ProccessReply的时候调用PlatformApplication->SetHighPrecisionMouseMode(false, nullptr);	PlatformApplication->SetCapture(nullptr);
		//调用了上面两句后，鼠标会被限制在一定范围。
		//而ProcessReply里仅仅有用的是MouseCaptor.InvalidateCaptureForPointer(UserIndex, PointerIndex);
		FWidgetPath EmptyWidgetPath;
		FSlateApplication::Get().ProcessReply(EmptyWidgetPath, Reply, nullptr, &PointerEvent, RayInput->GetUserIndex());
		if (Reply.IsEventHandled())
		{
			break;
		}
	}
#endif //USE_NEW_INPUT_SYSTEM
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