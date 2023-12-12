// Fill out your copyright notice in the Description page of Project Settings.


#include "InputModifierUtil.h"

#include "QuickLog.h"

FInputActionValue UInputModifierToAxisX::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                  FInputActionValue CurrentValue, float DeltaTime)
{
	if (CurrentValue.IsNonZero())
	{
		return FInputActionValue(CurrentValue.GetValueType(), FVector::XAxisVector);
	}
	else
	{
		return CurrentValue;
	}
}

FInputActionValue UInputModifierToAxisY::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                  FInputActionValue CurrentValue, float DeltaTime)
{
	if (CurrentValue.IsNonZero())
	{
		return FInputActionValue(CurrentValue.GetValueType(), FVector::YAxisVector);
	}
	else
	{
		return CurrentValue;
	}
}

FInputActionValue UInputModifierToAxisZ::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                                  FInputActionValue CurrentValue, float DeltaTime)
{
	if (CurrentValue.IsNonZero())
	{
		return FInputActionValue(CurrentValue.GetValueType(), FVector::ZAxisVector);
	}
	else
	{
		return CurrentValue;
	}
}

FInputActionValue UInputModifierLog::ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput,
                                                              FInputActionValue CurrentValue, float DeltaTime)
{
	LOG(TEXT("Action Value %s: %s"), *Prefix, *CurrentValue.ToString());
	return Super::ModifyRaw_Implementation(PlayerInput, CurrentValue, DeltaTime);
}
