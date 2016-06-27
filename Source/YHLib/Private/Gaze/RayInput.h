// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInput.generated.h"

UCLASS(Blueprintable,ClassGroup = (Input), meta = (BlueprintSpawnableComponent))
class URayInput : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	float RayLength;

	// Sets default values for this actor's properties
	URayInput();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	* only point event  (move)
	* TODO:suport Input Action
	*/
	virtual void Process();
	
	virtual bool GetRayPointer(FVector& RayStart, FVector& RayEnd);
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool GetHitResult(const FVector& RayStart,const FVector& RayEnd, const TArray<AActor*>& OptionalListOfIgnoredActors, FHitResult& Hit);	
	
	UFUNCTION(BlueprintCallable,Category="Input" )
	void SetCaster(USceneComponent* CasterComponent);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void AddIgnoreActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Input")
	void ClearIgnores();

protected:

	//caste ray component contain transform
	UPROPERTY()
	USceneComponent* Caster;

	UPROPERTY()
	UActorComponent* LastHitComponent;

	//FVector LastHitPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Input")
	TArray<AActor*> DefaultIgnores;
};
