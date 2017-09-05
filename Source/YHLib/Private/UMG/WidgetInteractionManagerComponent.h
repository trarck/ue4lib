// Fill out your copyright notice in the Description page of Project Settings.
/************************************************************************/
/*	work only  4.13 and later																			 */
/************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WidgetInteractionManagerComponent.generated.h"

class URayInput;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWidgetInteractionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetInteractionManagerComponent();
	
	UFUNCTION(BlueprintCallable, Category = "Gaze")
	virtual void RegisterProcess(class URayInput* InRayInput);

	UFUNCTION()
	void ProcessRayHit(bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit, bool bHaveRay);

	UFUNCTION()
	void OnKeyDown(const FKey& Key, bool bRepeat = false);

	UFUNCTION()
	void OnKeyUp(const FKey& Key);

	UFUNCTION()
	void OnProcessKeyChar(const FString& Characters,bool bRepeat);

	UFUNCTION()
	void OnPressPointerKey(const FKey& Key);

	UFUNCTION()
	void OnReleasePointerKey(const FKey& Key);

	bool IsHoverChanged();

	/**
	* Get the currently hovered widget component.
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
    UWidgetComponent* GetHoveredWidgetComponent() const;

	/**
	* Returns true if a widget under the hit result is interactive.  e.g. Slate widgets
	* that return true for IsInteractable().
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsOverInteractableWidget() const;

	/**
	* Returns true if a widget under the hit result is focusable.  e.g. Slate widgets that
	* return true for SupportsKeyboardFocus().
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsOverFocusableWidget() const;

	/**
	* Returns true if a widget under the hit result is has a visibility that makes it hit test
	* visible.  e.g. Slate widgets that return true for GetVisibility().IsHitTestVisible().
	*/
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	bool IsOverHitTestVisibleWidget() const;

	/**
	* Gets the widget path for the slate widgets under the last hit result.
	*/
	const FWeakWidgetPath& GetHoveredWidgetPath() const;

protected:
	
	bool bWidgetChange;

	UPROPERTY()
	URayInput* RayInput;

	FWeakWidgetPath LastWigetPath;

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

	/** The widget component we're currently hovering over. */
	UPROPERTY(Transient)
	UWidgetComponent* HoveredWidgetComponent;

	/** Are we hovering over any interactive widgets. */
	UPROPERTY(Transient)
	bool bIsHoveredWidgetInteractable;

	/** Are we hovering over any focusable widget? */
	UPROPERTY(Transient)
	bool bIsHoveredWidgetFocusable;

	/** Are we hovered over a widget that is hit test visible? */
	UPROPERTY(Transient)
	bool bIsHoveredWidgetHitTestVisible;
};