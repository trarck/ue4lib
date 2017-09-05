// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TestOverrideComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestOverrideComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestOverrideComponent();

	UFUNCTION(BlueprintCallable, Category = "Test")
	int OverFuna();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Test")
	void OverFunb(FVector& Vec);
	void OverFunb_Implementation(FVector& Vec);

protected:
};
