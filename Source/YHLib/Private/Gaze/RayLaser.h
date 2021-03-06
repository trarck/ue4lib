// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "RayLaser.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class URayLaser : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URayLaser();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable,Category="Laser")
	virtual void RegisterProcess(class URayInput* RayInput);

	UFUNCTION()
	void ProcessRayHit( bool bHit, const FVector&  Start, const FVector& End, const FHitResult& HitResult,bool bBeginHit,bool bHaveRay);

protected:

	void CreateLaserPointerMesh();

	void CreateHoverMesh();

	void CreateHoverPointLight();

	void LoadFromChildren();

	void SetLaserVisuals(const FLinearColor& NewColor, const float CrawlFade = 0.0f, const float CrawlSpeed = 0.0f);
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	float LaserPointerRadius;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	float LaserPointerHoverBallRadius;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	float LaserPointerLightRadius;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	float LaserPointerLightPullBackDistance;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	bool bPenetrate;

protected:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	UStaticMeshComponent* LaserPointerMeshComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	UStaticMeshComponent* HoverMeshComponent;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Laser")
	UPointLightComponent* HoverPointLightComponent;

	UPROPERTY()
	UMaterialInstanceDynamic* LaserPointerMID;
	
	UPROPERTY()
	UMaterialInstanceDynamic* TranslucentLaserPointerMID;

	float WorldScaleFactor;
};
