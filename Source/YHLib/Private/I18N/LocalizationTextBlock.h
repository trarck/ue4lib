// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "LocalizationTextBlock.generated.h"

/**
 * 
 */
UCLASS()
class ULocalizationTextBlock : public UTextBlock
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Localization)
	TMap<FString, FSlateFontInfo> CultureFonts;
	
public:
	virtual void PostLoad() override;

	UFUNCTION(BlueprintCallable,Category=Localization)
	void ChangeFontOfCulture(const FString& CultureName);

#if WITH_EDITOR
	virtual void OnSelectedByDesigner();
#endif
protected:
	void ChangeFontOfCurrentCulture();


};
