// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "YHUtils.generated.h"

/**
 * 
 */
UCLASS()
class UYHUtils : public UObject
{
	GENERATED_BODY()
public:
	static int32 GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects = nullptr);
};