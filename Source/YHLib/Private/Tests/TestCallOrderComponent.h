// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TestCallOrderComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestCallOrderComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="RayInput")
	USceneComponent* Sub;
	
	// Sets default values for this component's properties
	UTestCallOrderComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "RayInput")
	void MethadA();
	
	UFUNCTION(BlueprintCallable, Category = "RayInput")
	void MethadB();
		
protected:
	bool sign;
	
	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="RayInput")
	USceneComponent* CreateA;
};
