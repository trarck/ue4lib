// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Navigation/PathFollowingComponent.h"
#include "JumpPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class UJumpPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()
public:
	UJumpPathFollowingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadWrite, Category = "NavMesh")
	TEnumAsByte<enum EMovementMode> SpecailMovementMode;

	UPROPERTY(BlueprintReadWrite, Category = "NavMesh")
	TEnumAsByte<enum EMovementMode> NormalMovementMode;

protected:
	UPROPERTY(transient)
	UCharacterMovementComponent* CharacterMoveComp;
	
private:
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;	
};
