#pragma once

#include "CoreMinimal.h"

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