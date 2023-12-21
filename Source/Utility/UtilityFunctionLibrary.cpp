// Fill out your copyright notice in the Description page of Project Settings.


#include "UtilityFunctionLibrary.h"

#include "Math/UnrealMathUtility.h"

float UUtilityFunctionLibrary::LerpAlpha(float OldVal, float NewVal, float Alpha)
{
	static float CachedOldVal = 0.f;
	
	Alpha = FMath::Clamp(Alpha, 0.f, 1.f);
	if (FMath::IsNearlyZero(Alpha))
	{
		CachedOldVal = OldVal;
	}
	
	return FMath::Lerp(CachedOldVal, NewVal, Alpha);
}
