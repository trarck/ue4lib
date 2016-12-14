// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TestActorComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestActorComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UPROPERTY()
	int32 PublicPropOne;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Test")
	int32 PublicPropTwo;

	UFUNCTION()
	void PubFuna();

	UFUNCTION(BlueprintCallable,Category="Test")
	void PubFunb();

	UFUNCTION(BlueprintCallable, Category = "Test")
	int OverFuna();

	UFUNCTION(BlueprintNativeEvent, Category = "Test")
	void OverFunb(FVector& Vec);
	void OverFunb_Implementation(FVector& Vec);

protected:
	UFUNCTION()
	void ProFuna();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void ProFunb();

	UPROPERTY()
	int32 ProtecedPropOne;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Test")
	int32 ProtecedPropTwo;
};
