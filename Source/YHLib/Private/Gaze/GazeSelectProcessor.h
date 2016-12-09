// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazeSelectProcessor.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGazeSelectProcessor : public USceneComponent
{
	GENERATED_BODY()

public:	
	enum class EGazeState : uint8
	{
		None,
		//point is on the actor
		Stay,
		//after stay duration,action cut down
		Hover,
		//action is exec
		Actioned
	};
	
	// Sets default values for this component's properties
	UGazeSelectProcessor();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Gaze")
	virtual void RegisterProcess(class URayInput* RayInput);

	UFUNCTION()
	void ProcessRayHit( bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit,bool bHaveRay);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Gaze")
	void DoHoverStart();
	void DoHoverStart_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "Gaze")
	void DoHoverEnd();
	void DoHoverEnd_Implementation();
protected:
	void CreatePointerMesh();
	void CreateHoverMesh();
	void LoadFromChildren();

	void StartStay(bool bShowHover);

	void EndHover();

	void SetLaserVisuals(const FLinearColor& NewColor);
	void SetHoverPercent(float Percent);

protected:
	/**  protrude through  the actor*/
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	bool bPenetrate;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float PointerHoverRadius;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float ActionDuration;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	float StayDuration;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	FLinearColor GazeColor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	UStaticMeshComponent* PointerMeshComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Gaze")
	UStaticMeshComponent* HoverMeshComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* PointerMID;

	UPROPERTY()
	UMaterialInstanceDynamic* HoverMID;

	UPROPERTY()
	class URayInteractiveComponent* GazeInteractiveComponent;

	//ref to RayInput
	UPROPERTY()
	class URayInput* RefRayInput;

	float Elapsed;

	float Duration;

	bool bChangeColor;

	EGazeState State;
};
