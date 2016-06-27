// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "TestFindComponent.generated.h"

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestParentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestParentComponent()
	{}


protected:
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestSubComponent : public UTestParentComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestSubComponent()
	{}

protected:
};

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTestFindComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTestFindComponent();

	UFUNCTION(BlueprintCallable, Category = "Test")
	int FindSubComponent();
	

protected:
};
