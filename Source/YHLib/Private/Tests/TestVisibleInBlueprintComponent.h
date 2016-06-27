// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TestVisibleInBlueprintComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestVisibleInBlueprintComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestVisibleInBlueprintComponent();

	UPROPERTY()
	int32 PublicPropOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Test")
	int32 PublicPropTwo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere,AdvancedDisplay, Category = "Test")
	int32 PublicPropThree;

	UFUNCTION()
	void PubFuna();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void PubFunb();

protected:

	UFUNCTION()
	void ProFuna();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void ProFunb();

	UPROPERTY()
	int32 ProtecedPropOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Test")
	int32 ProtecedPropTwo;
};
