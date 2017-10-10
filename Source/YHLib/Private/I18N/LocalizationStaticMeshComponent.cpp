// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalizationStaticMeshComponent.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"
#include "Engine/StaticMesh.h"

DEFINE_LOG_CATEGORY_STATIC(LogYHI18N, Log, All);

//ULocalizationStaticMeshComponent::ULocalizationStaticMeshComponent(const FObjectInitializer& ObjectInitializer)
//	: Super(ObjectInitializer)
//{
//	UE_LOG(LogYHI18N, Log, TEXT("ULocalizationStaticMeshComponent::ULocalizationStaticMeshComponent"));
//}

void ULocalizationStaticMeshComponent::PostLoad()
{
	Super::PostLoad();
	UE_LOG(LogYHI18N, Log, TEXT("ULocalizationStaticMeshComponent::PostLoad"));
	FString CurrentCultureName = FInternationalization::Get().GetCurrentCulture()->GetName();
	ChangeStaticMeshOfCulture(CurrentCultureName);
}

//void ULocalizationStaticMeshComponent::PostInitProperties()
//{
//	Super::PostInitProperties();
//	UE_LOG(LogYHI18N, Log, TEXT("ULocalizationStaticMeshComponent::PostInitProperties"));
//}
//
//void ULocalizationStaticMeshComponent::Serialize(FArchive& Ar)
//{
//	Super::Serialize(Ar);
//	UE_LOG(LogYHI18N, Log, TEXT("ULocalizationStaticMeshComponent::Serialize"));
//}

void ULocalizationStaticMeshComponent::ChangeStaticMeshOfCulture(const FString& CultureName)
{
	const TArray<FString> PrioritizedCultureNames = FInternationalization::Get().GetPrioritizedCultureNames(CultureName);
	for (const FString& PrioritizedCultureName : PrioritizedCultureNames)
	{
		UStaticMesh* StaticMesh = CultureStaticMeshs.FindRef(PrioritizedCultureName);
		if (StaticMesh) {
			SetStaticMesh(StaticMesh);
			break;
		}
	}
}