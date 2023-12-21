// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UtilityFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UTILITY_API UUtilityFunctionLibrary : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static float LerpAlpha(float OldVal, float NewVal, float Alpha);
};
