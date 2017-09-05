// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "LocalizationImage.generated.h"

/**
 * 
 */
UCLASS()
class ULocalizationImage : public UImage
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Localization)
	TMap<FString, FSlateBrush> CultureBrushs;

public:
	virtual void PostLoad();

	UFUNCTION(BlueprintCallable, Category = Localization)
	void ChangeBrushOfCulture(const FString& CultureName);
protected:
	void ChangeBrushOfCurrentCulture();
	
};
