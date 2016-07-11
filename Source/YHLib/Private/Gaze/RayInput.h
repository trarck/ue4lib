// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FProcessRaySignature,bool,bHit,const FVector&,Start,const FVector&,End ,const FHitResult&, HitResult, bool, bBeginHit,bool,bHaveRay);

UCLASS(Blueprintable,ClassGroup = (Input), meta = (BlueprintSpawnableComponent))
class URayInput : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	float RayLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser")
	bool bIgnoreSelf;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FProcessRaySignature OnProcessRay;

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
	UFUNCTION(BlueprintCallable, Category = "Laser")
	bool GetHitResult(const FVector& RayStart,const FVector& RayEnd, const TArray<AActor*>& OptionalListOfIgnoredActors, FHitResult& Hit);	
	
	/** Set caster componet. normal is ray come from*/
	UFUNCTION(BlueprintCallable,Category="Laser" )
	void SetCaster(USceneComponent* CasterComponent);

	UFUNCTION(BlueprintCallable, Category = "Laser")
	void AddIgnoreActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category = "Laser")
	void ClearIgnores();

	//handle Input key event
	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool HandleKeyDown(FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool HandleKeyUp(FKey Key);

	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool HandleKeyDownEvent(FKeyEvent KeyEvent);

	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool HandleKeyUpEvent(FKeyEvent KeyEvent);

protected:

	//Actors don't check
	UPROPERTY(BlueprintReadWrite, Category = "Laser")
	TArray<AActor*> DefaultIgnores;

	//caste ray component contain transform. maybe camera,vr handle or others.
	//UPROPERTY()
	USceneComponent* Caster;

	UPROPERTY()
	UActorComponent* LastHitComponent;

	UPROPERTY()
	class URayInteractiveComponent* CurrentInteractiveComponent;

	//FVector LastHitPoint;

};
