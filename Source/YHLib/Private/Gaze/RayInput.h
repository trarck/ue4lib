// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInput.generated.h"

UCLASS(Blueprintable,ClassGroup = (Input), meta = (BlueprintSpawnableComponent))
class URayInput : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RayInput")
	float RayLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RayInput")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RayInput")
	bool bIgnoreSelf;

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
	
	/** Calc ray start point and end point .use for screen component as caster define*/
	virtual bool GetRayPointer(FVector& RayStart, FVector& RayEnd);
	
	/**
	* Get collision hit result
	* @param	RayStart	Start point
	* @param	RayEnd 	End point
	* @param	OptionalListOfIgnoredActors 	Actors not collions .
	* @param Hit HitReult
	* @return 	HaveHit bool
	*/
	UFUNCTION(BlueprintCallable, Category = "RayInput")
	bool GetHitResult(const FVector& RayStart,const FVector& RayEnd, const TArray<AActor*>& OptionalListOfIgnoredActors, FHitResult& Hit);	
	
	/** Set caster componet. normal is ray come from*/
	UFUNCTION(BlueprintCallable,Category="RayInput" )
	void SetCaster(USceneComponent* CasterComponent);

	UFUNCTION(BlueprintCallable, Category = "RayInput")
	void AddIgnoreActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "RayInput")
	void ClearIgnores();

protected:

	//caste ray component contain transform. mustbe camera,vr handle or others.
	UPROPERTY()
	USceneComponent* Caster;

	UPROPERTY()
	UActorComponent* LastHitComponent;

	//FVector LastHitPoint;

	//Actors don't check
	UPROPERTY(BlueprintReadWrite, Category = "RayInput")
	TArray<AActor*> DefaultIgnores;
};
