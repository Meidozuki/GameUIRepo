// Fill out your copyright notice in the Description page of Project Settings.


#include "LerpUtil.h"

#include "Math/UnrealMathUtility.h"

bool ULerpUtilFunctionLibrary::MaybeLerp(FLerpUtil LerpUtil, float OldVal, float NewVal, float DeltaTime, float& OutVal)
{
	if (FMath::IsNearlyEqual(OldVal, NewVal, LerpUtil.Epsilon))
	{
		OutVal = NewVal;
		return false;
	}
	else
	{
		OutVal = FMath::FInterpConstantTo(OldVal, NewVal, DeltaTime, LerpUtil.InterpSpeed);
		return true;
	}
}

bool ULerpUtilFunctionLibrary::MaybeLerp_NoTargetArg(FLerpUtil LerpUtil, float OldVal, float DeltaTime, float& OutVal)
{
	return MaybeLerp(LerpUtil, OldVal, LerpUtil.Target, DeltaTime, OutVal);
}

void ULerpUtilFunctionLibrary::SetNewVal(FLerpUtil& LerpUtil, float NewVal)
{
	LerpUtil.Target = NewVal;
}

