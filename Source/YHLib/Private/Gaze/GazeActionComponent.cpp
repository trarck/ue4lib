// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "GazeActionComponent.h"


// Sets default values for this component's properties
UGazeActionComponent::UGazeActionComponent()
:ActionDuration(0.0f),
bHaveGazeColor(false)
{

}

bool UGazeActionComponent::RestartHover()
{
	return false;
}

bool UGazeActionComponent::Hoverable()
{
	return true;
}