// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "JumpPathFollowingComponent.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "NavTypes.h"
#include "NavUtils.h"

DEFINE_LOG_CATEGORY_STATIC(JumpPathFollowingComponent, Log, All);

UJumpPathFollowingComponent::UJumpPathFollowingComponent(const FObjectInitializer& ObjectInitializer /* = FObjectInitializer::Get() */)
	:Super(ObjectInitializer),
	SpecailMovementMode(MOVE_Flying),
	NormalMovementMode(MOVE_Walking),
	CharacterMoveComp(nullptr)
{

}

void UJumpPathFollowingComponent::SetMoveSegment(int32 SegmentStartIndex)
{
	Super::SetMoveSegment(SegmentStartIndex);
	
	float t = GetWorld()->GetRealTimeSeconds();
	UE_LOG(JumpPathFollowingComponent, Log, TEXT("[%f]SetMoveSegment:%d"),t,SegmentStartIndex);
	if (CharacterMoveComp != nullptr)
	{
		const FNavPathPoint& SegmentStart = Path->GetPathPoints()[MoveSegmentStartIndex];
		if (NavUtils::HasJumpFlag(SegmentStart))//FNavMeshNodeFlags(SegmentStart.Flags).AreaFlags & (1 << (uint8)ENavAreaFlag::Jump)
		{
			UE_LOG(JumpPathFollowingComponent, Log, TEXT("[%f]SetMoveSegment:Fly"),t, SegmentStartIndex);
			CharacterMoveComp->SetMovementMode(SpecailMovementMode);
		}
		else
		{
			UE_LOG(JumpPathFollowingComponent, Log, TEXT("[%f]SetMoveSegment:Walk"), t,SegmentStartIndex);
			CharacterMoveComp->SetMovementMode(NormalMovementMode);
		}
	}
}

void UJumpPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);
	CharacterMoveComp = Cast<UCharacterMovementComponent>(MoveComp);
}

