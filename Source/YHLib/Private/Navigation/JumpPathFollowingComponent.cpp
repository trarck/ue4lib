// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpPathFollowingComponent.h"
#include "Engine.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "NavTypes.h"
#include "NavUtils.h"

DEFINE_LOG_CATEGORY_STATIC(JumpPathFollowingComponent, Log, All);

UJumpPathFollowingComponent::UJumpPathFollowingComponent(const FObjectInitializer& ObjectInitializer /* = FObjectInitializer::Get() */)
	:Super(ObjectInitializer),
	SpecailMovementMode(MOVE_Flying),
	NormalMovementMode(MOVE_Walking),
	CharacterMoveComp(nullptr),
	bInJumpNavArea(false)
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
			bInJumpNavArea = true;
			UE_LOG(JumpPathFollowingComponent, Log, TEXT("[%f]SetMoveSegment:Fly"),t, SegmentStartIndex);
			CharacterMoveComp->SetMovementMode(SpecailMovementMode);
			OnEnterJumpArea.Broadcast(CharacterMoveComp);
		}
		else
		{
			UE_LOG(JumpPathFollowingComponent, Log, TEXT("[%f]SetMoveSegment:Walk"), t, SegmentStartIndex);
			if (bInJumpNavArea)
			{
				bInJumpNavArea = false;				
				CharacterMoveComp->SetMovementMode(NormalMovementMode);
				OnLeaveJumpArea.Broadcast(CharacterMoveComp);
			}
		}
	}
}

void UJumpPathFollowingComponent::SetMovementComponent(UNavMovementComponent* MoveComp)
{
	Super::SetMovementComponent(MoveComp);
	CharacterMoveComp = Cast<UCharacterMovementComponent>(MoveComp);
}

