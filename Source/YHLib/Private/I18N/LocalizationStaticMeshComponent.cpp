// Fill out your copyright notice in the Description page of Project Settings.

#include "LocalizationStaticMeshComponent.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

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