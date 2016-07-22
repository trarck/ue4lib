// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "NavTypes.h"

class NavUtils
{
public:
	FORCEINLINE static bool IsSet(uint16 Flags, ENavAreaFlag Bit) { return (Flags & (1 << (uint8)Bit)) != 0; }
	FORCEINLINE static void Set(uint16& Flags, ENavAreaFlag Bit) { Flags |= (1 << (uint8)Bit); }

	FORCEINLINE static bool IsNavLink(const FNavPathPoint& PathVert) { return (FNavMeshNodeFlags(PathVert.Flags).PathFlags & RECAST_STRAIGHTPATH_OFFMESH_CONNECTION) != 0; }
	FORCEINLINE static bool HasJumpFlag(const FNavPathPoint& PathVert) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Jump); }
	FORCEINLINE static bool HasCrouchFlag(const FNavPathPoint& PathVert) { return IsSet(FNavMeshNodeFlags(PathVert.Flags).AreaFlags, ENavAreaFlag::Crouch); }
};