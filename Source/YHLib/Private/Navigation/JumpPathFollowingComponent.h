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

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnterJumpAreaSignature, UCharacterMovementComponent*, CharacterMovementComponent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLeaveJumpAreaSignature, UCharacterMovementComponent*, CharacterMovementComponent);

public:
	UJumpPathFollowingComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadWrite, Category = "NavMesh")
	TEnumAsByte<enum EMovementMode> SpecailMovementMode;

	UPROPERTY(BlueprintReadWrite, Category = "NavMesh")
	TEnumAsByte<enum EMovementMode> NormalMovementMode;

	UPROPERTY(BlueprintAssignable, Category = "NavMesh")
	FEnterJumpAreaSignature OnEnterJumpArea;

	UPROPERTY(BlueprintAssignable, Category = "NavMesh")
	FLeaveJumpAreaSignature OnLeaveJumpArea;

protected:
	UPROPERTY(transient)
	UCharacterMovementComponent* CharacterMoveComp;

	bool bInJumpNavArea;
private:
	virtual void SetMoveSegment(int32 SegmentStartIndex) override;
	virtual void SetMovementComponent(UNavMovementComponent* MoveComp) override;	
};
