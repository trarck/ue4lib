// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "GazePoint.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UGazePoint : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGazePoint();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category="Laser")
	virtual void RegisterProcess(class URayInput* RayInput);

	UFUNCTION()
	void ProcessRayHit( bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult,bool bHaveRay);

protected:
	void CreatePointerMesh();
	void CreateHoverMesh();
	void LoadFromChildren();
	void UGazePoint::SetLaserVisuals(const FLinearColor& NewColor);
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	float PointerHoverRadius;

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	UStaticMeshComponent* PointerMeshComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	UStaticMeshComponent* HoverMeshComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* PointerMID;

	UPROPERTY()
	UMaterialInstanceDynamic* HoverMID;
};
