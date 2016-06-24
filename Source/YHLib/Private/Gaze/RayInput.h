// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInput.generated.h"

UCLASS(Blueprintable,ClassGroup = (Input), meta = (BlueprintSpawnableComponent))
class URayInput : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	URayInput();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Process();
	
	virtual bool GetRayPointer(FVector& RayStart, FVector& RayEnd);
	
	UFUNCTION(BlueprintCallable, Category = "Input")
	bool GetHitResult(const FVector& RayStart,const FVector& RayEnd, const TArray<AActor*>& OptionalListOfIgnoredActors, FHitResult& Hit);	
	
	UFUNCTION(BlueprintCallable,Category="Input" )
	void SetCaster(USceneComponent* CasterComponent);

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Input")
	float RayLength;

	//caste ray component contain transform
	UPROPERTY()
	USceneComponent* Caster;

	UPROPERTY()
	AActor* LastHitActor;
};
