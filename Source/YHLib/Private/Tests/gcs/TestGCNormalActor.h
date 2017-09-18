// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestGCNormalActor.generated.h"

class UTestGCActorComponent;
class UTestGCObject;

UCLASS(Blueprintable)
class ATestGCNormalActor : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY()
	UTestGCActorComponent* FirstComponent;

	UTestGCActorComponent* SecondComponent;

	UPROPERTY()
	UTestGCObject* OneObject;

	UTestGCObject* TwoObject;

	UPROPERTY()
	TArray<UTestGCObject*> MyFirstObjs;

	TArray<UTestGCObject*> MySecondObjs;

	UPROPERTY()
	TMap<FString, UTestGCObject*> MyFirstMaps;
	TMap<FString, UTestGCObject*> MySecondMaps;
public:	
	// Sets default values for this actor's properties
	ATestGCNormalActor();
	virtual ~ATestGCNormalActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
