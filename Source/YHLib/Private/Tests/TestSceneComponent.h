// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TestSceneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestSceneComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestSceneComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "RayInput")
	void CreateComponents();
		
protected:
	UPROPERTY()
	UStaticMeshComponent* LaserPointerMeshComponent;

	UPROPERTY()
	UStaticMeshComponent* HoverMeshComponent;

	UPROPERTY()
	UPointLightComponent* HoverPointLightComponent;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="RayInput")
	float LaserPointerLightRadius;
};
