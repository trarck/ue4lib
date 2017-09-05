// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalizationFunctionLibrary.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

FString ULocalizationFunctionLibrary::GetCurrentCulture()
{
	return FInternationalization::Get().GetCurrentCulture()->GetName();
}

TArray<FString> ULocalizationFunctionLibrary::GetPrioritizedCultureNames(const FString& culture)
{
	return FInternationalization::Get().GetPrioritizedCultureNames(culture);
}

TArray<FString> ULocalizationFunctionLibrary::GetCurrentPrioritizedCultureNames()
{
	return FInternationalization::Get().GetPrioritizedCultureNames(FInternationalization::Get().GetCurrentCulture()->GetName());
}