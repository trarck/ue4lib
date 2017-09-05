// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalizationTextBlock.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Internationalization/Culture.h"

#if WITH_EDITOR
#include "LocalizationSettings.h"
#endif

void ULocalizationTextBlock::PostLoad()
{
	Super::PostLoad();
	ChangeFontOfCurrentCulture();
}

void ULocalizationTextBlock::ChangeFontOfCurrentCulture()
{
	//get current culture
	FString CurrentCultureName = FInternationalization::Get().GetCurrentCulture()->GetName();
	ChangeFontOfCulture(CurrentCultureName);
	/*const TArray<FString> PrioritizedCultureNames =FInternationalization::Get().GetPrioritizedCultureNames(CurrentCultureName);
	for (const FString& PrioritizedCultureName : PrioritizedCultureNames)
	{
		FSlateFontInfo* FontInfo = CultureFonts.Find(PrioritizedCultureName);
		if (FontInfo) {
			SetFont(FontInfo);
			break;
		}
	}*/
}

void ULocalizationTextBlock::ChangeFontOfCulture(const FString& CultureName)
{
	const TArray<FString> PrioritizedCultureNames = FInternationalization::Get().GetPrioritizedCultureNames(CultureName);
	for (const FString& PrioritizedCultureName : PrioritizedCultureNames)
	{
		FSlateFontInfo* FontInfo = CultureFonts.Find(PrioritizedCultureName);
		if (FontInfo) {
			SetFont(*FontInfo);
			break;
		}
	}
}

#if WITH_EDITOR
void ULocalizationTextBlock::OnSelectedByDesigner()
{
	if (CultureFonts.Num())
	{
		ULocalizationTargetSet* const TargetSet = ULocalizationSettings::GetGameTargetSet();
		for (auto& Target : TargetSet->TargetObjects)
		{
			for (FCultureStatistics& Stat : Target->Settings.SupportedCulturesStatistics)
			{
				if (CultureFonts.Contains(Stat.CultureName)) continue;
				CultureFonts.Add(Stat.CultureName, Font);
			}
		}
	}
}
#endif