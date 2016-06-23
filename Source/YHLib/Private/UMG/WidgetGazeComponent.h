// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Gaze/GazeComponent.h"
#include "WidgetGazeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWidgetGazeComponent : public UGazeComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWidgetGazeComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable,Category="Gaze")
	void OnGaze(FVector WorldLocation,class UActorComponent* ActorComponent);
	
	virtual void OnGazeEnter(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void OnGazeStay(FVector HitLocation, UActorComponent* HitComponent, const FHitResult& Hit) override;

	virtual void OnGazeExit(FVector HitLocation, UActorComponent* HitComponent) override;

protected:
};
