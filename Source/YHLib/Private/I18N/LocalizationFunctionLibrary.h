// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LocalizationFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ULocalizationFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
		UFUNCTION(BlueprintCallable, Category = Localization)
		static FString GetCurrentCulture();

		UFUNCTION(BlueprintCallable, Category = Localization)
		static TArray<FString> GetPrioritizedCultureNames(const FString& culture);
	
		UFUNCTION(BlueprintCallable, Category = Localization)
		static TArray<FString> GetCurrentPrioritizedCultureNames();
};
