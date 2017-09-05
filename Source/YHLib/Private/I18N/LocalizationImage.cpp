// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalizationImage.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"


void ULocalizationImage::PostLoad()
{
	Super::PostLoad();
    ChangeBrushOfCurrentCulture();
}


void ULocalizationImage::ChangeBrushOfCurrentCulture()
{
	//get current culture
	FString CurrentCultureName = FInternationalization::Get().GetCurrentCulture()->GetName();
	ChangeBrushOfCulture(CurrentCultureName);
}

void ULocalizationImage::ChangeBrushOfCulture(const FString& CultureName)
{
	const TArray<FString> PrioritizedCultureNames = FInternationalization::Get().GetPrioritizedCultureNames(CultureName);
	for (const FString& PrioritizedCultureName : PrioritizedCultureNames)
	{
		FSlateBrush* Brush = CultureBrushs.Find(PrioritizedCultureName);
		if (Brush) {
			SetBrush(*Brush);
			break;
		}
	}
}