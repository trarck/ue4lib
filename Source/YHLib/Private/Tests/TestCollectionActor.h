// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TestCollectionActor.generated.h"

UCLASS(Blueprintable)
class ATestCollectionActor : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<class UStaticMesh*> ArrayItems;

	UPROPERTY()
	TSet<class UStaticMesh*> SetItems;

	UPROPERTY()
	TMap< FString, class UStaticMesh*> MapItems;

	TArray<class UStaticMesh*> NormalArrayItems;

	TSet<class UStaticMesh*> NormalSetItems;

	TMap< FString, class UStaticMesh*> NormalMapItems;

	UPROPERTY()
	TArray<class UObject*> ArrayObjs;

	TArray<class UObject*> NormalArrayObjs;

	UPROPERTY()
	class UTestItemComponent* FirstTestItemComponent;

	class UTestItemComponent* SecondTestItemComponent;
public:	
	// Sets default values for this actor's properties
	ATestCollectionActor();
	virtual ~ATestCollectionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Test")
	void AddStaticMeshs();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void RemoveStaticMeshs();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void AddObjs();

	UFUNCTION(BlueprintCallable, Category = "Test")
	void RemoveObjs();

protected:
	void ShowRefs(UObject* obj);
	
	float Elapsed;

	int Seq;
};
