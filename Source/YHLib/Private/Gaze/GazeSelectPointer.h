// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazeSelectPointer.generated.h"


enum class EGazeSelectState : uint8
{
	None,
	//after stay duration,action cut down
	Hover,
	Hovered
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGazeSelectPointer : public USceneComponent
{
	GENERATED_BODY()

public:	

	// Sets default values for this component's properties
	UGazeSelectPointer();

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable,Category="Gaze")
	virtual void RegisterProcess(class URayInput* RayInput);

	UFUNCTION()
	void ProcessRayHit( bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult, bool bBeginHit,bool bHaveRay);

protected:
	void CreatePointerMesh();
	void CreateHoverMesh();
	void LoadFromChildren();

	void StartStay();

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
	class UGazeActionComponent* GazeActionComponent;

	float Elapsed;

	float Duration;

	bool bChangeColor;

	EGazeSelectState State;
};
