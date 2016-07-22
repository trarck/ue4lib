// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "NavAreaJump.h"
#include "NavTypes.h"
#include "NavUtils.h"

UNavAreaJump::UNavAreaJump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	NavUtils::Set(AreaFlags, ENavAreaFlag::Jump);
}

