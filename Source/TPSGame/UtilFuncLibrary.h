// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UtilFuncLibrary.generated.h"

/**
 * 
 */
UCLASS()
class UUtilityFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	static FVector RotateXYToScreenSpace(const FVector& InVector) {
		return FVector(InVector.Z, InVector.X, InVector.Y);
	}
	
	UFUNCTION(BlueprintCallable)
	static FVector RotateScreenToXYSpace(const FVector& InVector) {
		return FVector(InVector.Y, InVector.Z, InVector.X);
	}
};
