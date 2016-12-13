// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayInput.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FProcessRaySignature,bool,bHit,const FVector&,Start,const FVector&,End ,const FHitResult&, HitResult, bool, bBeginHit,bool,bHaveRay);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPressKeySignature, const FKey& ,Key, bool ,bRepeat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReleaseKeySignature, const FKey&, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSendKeyCharSignature, const FString& ,Characters, bool, bRepeat);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPressPointerKeySignature, const FKey&, Key);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReleasePointerKeySignature, const FKey&, Key);

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

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FPressKeySignature OnPressKey;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FReleaseKeySignature OnReleaseKey;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FSendKeyCharSignature OnSendKeyChar;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FPressPointerKeySignature OnPressPointerKey;

	UPROPERTY(BlueprintAssignable, Category = "Laser")
	FReleasePointerKeySignature OnReleasePointerKey;

	/**
	* Represents the Virtual User Index.  Each virtual user should be represented by a different
	* index number, this will maintain separate capture and focus states for them.  Each
	* controller or finger-tip should get a unique PointerIndex.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser", meta = (ClampMin = "0", ExposeOnSpawn = true))
	int32 VirtualUserIndex;

	/**
	* Each user virtual controller or virtual finger tips being simulated should use a different pointer index.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Laser", meta = (ClampMin = "0", UIMin = "0", UIMax = "9", ExposeOnSpawn = true))
	int32 PointerIndex;

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

	/**
     * handle key press by the keyboard
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool PressKey(const FKey& Key,bool bRepeat = false);

	/**
	 * handle key release by the keyboard
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual bool ReleaseKey(const FKey& Key);

	/**
	* Transmits a list of characters to a widget by simulating a OnKeyChar event for each key listed in
	* the string.
	*/
	UFUNCTION(BlueprintCallable, Category = "Laser")
	bool SendKeyChar(const FString& Characters, bool bRepeat);

	/**
	 * Presses a key as if the mouse/pointer were the source of it.  Normally you would just use
	 * Left/Right mouse button for the Key.  However - advanced uses could also be imagined where you
	 * send other keys to signal widgets to take special actions if they're under the cursor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual void PressPointerKey(const FKey& Key);

	/**
	 * Presses a key as if the mouse/pointer were the source of it.  Normally you would just use
	 * Left/Right mouse button for the Key.  However - advanced uses could also be imagined where you
	 * send other keys to signal widgets to take special actions if they're under the cursor.
	 */
	UFUNCTION(BlueprintCallable, Category = "Laser")
	virtual void ReleasePointerKey(const FKey& Key);

	int32 GetUserIndex();

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
#if USE_NEW_INPUT_SYSTEM
	TSharedPtr<FSlateVirtualUser> VirtualUser;
#endif //USE_NEW_INPUT_SYSTEM
};
