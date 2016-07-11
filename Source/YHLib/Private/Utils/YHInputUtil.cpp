// Fill out your copyright notice in the Description page of Project Settings.

#include "YHLibPrivatePCH.h"
#include "YHInputUtil.h"

#include "Engine/InputDelegateBinding.h"
#include "Components/InputComponent.h"

UInputComponent* UYHInputUtil::CreateInputComponent(AActor* Owner, FName Name, bool bBlockInput, int32 InputPriority)
{
	UInputComponent* InputComponent = NewObject<UInputComponent>(Owner, Name);
	InputComponent->RegisterComponent();
	InputComponent->bBlockInput = bBlockInput;
	InputComponent->Priority = InputPriority;
	if (UInputDelegateBinding::SupportsInputDelegate(Owner->GetClass()))
	{
		UInputDelegateBinding::BindInputDelegates(Owner->GetClass(), InputComponent);
	}

	return InputComponent;
}

void UYHInputUtil::PushInputComponent(APlayerController* PlayerController, UInputComponent* InputComponent)
{
	if (PlayerController)
	{
		if (InputComponent)
		{
			PlayerController->PushInputComponent(InputComponent);
		}
	}
}

void UYHInputUtil::PopInputComponent(APlayerController* PlayerController, UInputComponent* InputComponent)
{
	if (PlayerController)
	{
		if (InputComponent)
		{
			PlayerController->PopInputComponent(InputComponent);
		}
	}
}

const FKey UYHInputUtil::GetInputKey(FName Name)
{
	TArray<FKey> AllKeys;
	EKeys::GetAllKeys(AllKeys);
	for (int i = 0; i < AllKeys.Num(); ++i)
	{
		if (AllKeys[i].GetFName().IsEqual(Name))
		{
			return AllKeys[i];
		}
	}
	return EKeys::Invalid;
}



