// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "WidgetGazeActionComponent.h"


// Sets default values for this component's properties
UWidgetGazeActionComponent::UWidgetGazeActionComponent()
	:bWidgetHover(false),
	bWidgetHoverable(false)
{

}

bool UWidgetGazeActionComponent::RestartHover()
{
	return bWidgetHover;
}

bool UWidgetGazeActionComponent::Hoverable()
{
	return bWidgetHoverable;
}