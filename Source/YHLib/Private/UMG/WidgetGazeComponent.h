// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Gaze/GazeInteractiveComponent.h"
#include "WidgetGazeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWidgetGazeComponent : public UGazeInteractiveComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetGazeComponent();
	
	virtual void RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput) override;

	virtual void RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit, URayInput* RayInput) override;

	virtual void RayExit(UActorComponent* HitComponent, URayInput* RayInput) override;

	virtual void KeyDown(FKey Key, URayInput* RayInput, bool bRepeat = false) override;

	virtual void KeyUp(FKey Key, URayInput* RayInput) override;

	virtual void KeyDownEvent(FKeyEvent KeyEvent, URayInput* RayInput) override;

	virtual void KeyUpEvent(FKeyEvent KeyEvent, URayInput* RayInput) override;

	virtual void PressPointerKey(FKey Key, URayInput* RayInput) override;

	virtual void ReleasePointerKey(FKey Key, URayInput* RayInput) override;

	virtual bool IsHover() override;

	virtual bool IsHoverChanged() override;

	//default is button
	virtual bool IsWidgetInteractive(TSharedRef<SWidget> Widget);

	const FWeakWidgetPath& GetHoveredWidgetPath() const;

	FORCEINLINE const FWidgetAndPointer& GetActiveWidgetAndPointer()
	{
		return ActiveWidgetAndPointer;
	}

protected:
	bool bWidgetHover;
	bool bWidgetChange;
	
	FWeakWidgetPath LastWigetPath;

	FWidgetAndPointer ActiveWidgetAndPointer;
	//TWeakPtr<SWidget> ActiveWidget;

	/** The modifier keys to simulate during key presses. */
	FModifierKeysState ModifierKeys;

	/** The current set of pressed keys we maintain the state of. */
	TSet<FKey> PressedKeys;

	/** The 2D location on the widget component that was hit. */
	UPROPERTY(Transient)
	FVector2D LocalHitLocation;

	/** The last 2D location on the widget component that was hit. */
	UPROPERTY(Transient)
	FVector2D LastLocalHitLocation;
};
