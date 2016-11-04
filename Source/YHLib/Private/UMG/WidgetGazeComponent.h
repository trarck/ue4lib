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
	
	virtual void RayEnter(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void RayStay(const FVector& HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void RayExit(UActorComponent* HitComponent) override;

	virtual void KeyDown(FKey Key, bool bRepeat = false) override;

	virtual void KeyUp(FKey Key) override;

	virtual void KeyDownEvent(FKeyEvent KeyEvent) override;

	virtual void KeyUpEvent(FKeyEvent KeyEvent) override;

	virtual bool IsHover() override;

	virtual bool IsHoverChanged() override;

	//default is button
	virtual bool IsWidgetInteractive(TSharedRef<SWidget> Widget);

	FORCEINLINE const FWidgetAndPointer& GetActiveWidgetAndPointer()
	{
		return ActiveWidgetAndPointer;
	}
protected:
	bool bWidgetHover;
	bool bWidgetChange;
	FWidgetAndPointer ActiveWidgetAndPointer;
};
