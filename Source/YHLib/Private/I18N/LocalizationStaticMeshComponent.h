// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "LocalizationStaticMeshComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Rendering, Common), hidecategories = (Object, Activation, "Components|Activation"), ShowCategories = (Mobility), editinlinenew, meta = (BlueprintSpawnableComponent))
class ULocalizationStaticMeshComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	//GENERATED_UCLASS_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Localization)
	TMap<FString, class UStaticMesh*> CultureStaticMeshs;
public:
	virtual void PostLoad() override;

	//virtual void PostInitProperties() override;	

	//virtual void Serialize(FArchive& Ar) override;
};
