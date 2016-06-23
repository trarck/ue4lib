// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "YHInputUtil.generated.h"

/**
 * 
 */
UCLASS()
class UYHInputUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
		static class UInputComponent* CreateInputComponent(AActor* Onwer, FName Name, bool bBlockInput = true, int32 InputPriority = 0);

	UFUNCTION(BlueprintCallable, Category = Input)
		static void PushInputComponent(APlayerController* PlayerController, UInputComponent* InputComponent);

	UFUNCTION(BlueprintCallable, Category = Input)
		static void PopInputComponent(APlayerController* PlayerController, UInputComponent* InputComponent);
	
	
};
