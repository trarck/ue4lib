// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazePointer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGazePointer : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGazePointer();

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
	void StartHover();
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

	bool bIsHover;

	float Duration;

	bool bChangeColor;

	bool bShowHover;
};
