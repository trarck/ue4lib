// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestGCObject.generated.h"

/**
 * 
 */
UCLASS()
class UTestGCObject : public UObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UTestGCObject();
	~UTestGCObject();
	
	virtual void BeginDestroy();
	virtual void FinishDestroy();

	FName MyName;
};
