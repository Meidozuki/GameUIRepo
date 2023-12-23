// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LerpUtil.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UTILITY_API FLerpUtil
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Epsilon = UE_SMALL_NUMBER;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InterpSpeed = 1.0f;

	UPROPERTY(BlueprintReadWrite)
	float Target;
};

UCLASS()
class UTILITY_API ULerpUtilFunctionLibrary: public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure, DisplayName="MaybeLerp (With NewVal)")
	static UPARAM(DisplayName="IsLerping") bool
	MaybeLerp(FLerpUtil LerpUtil, float OldVal, float NewVal, float DeltaTime, float& OutVal);
	
	UFUNCTION(BlueprintPure, DisplayName="MaybeLerp (Without NewVal)")
	static UPARAM(DisplayName="IsLerping") bool
	MaybeLerp_NoTargetArg(FLerpUtil LerpUtil, float OldVal, float DeltaTime, float& OutVal);

	UFUNCTION(BlueprintCallable)
	static void SetNewVal(UPARAM(ref) FLerpUtil& LerpUtil, float NewVal);
};
