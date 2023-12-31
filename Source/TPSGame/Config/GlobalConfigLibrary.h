﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GlobalConfigTypes.h"
#include "GlobalConfigLibrary.generated.h"


UENUM()
enum class EGridAlignAxis { AxisX, AxisY, AxisZ };


UCLASS()
class TPSGAME_API UGlobalConfigLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
public:
	UFUNCTION(BlueprintPure)
	static EGridAlignAxis GetAlignAxis() { return EGridAlignAxis::AxisX;}
	
	UFUNCTION(BlueprintPure)
	static FOptionalFloat ReadFloatConfig(FName Name);
	
	UFUNCTION(BlueprintCallable)
	static inline float ReadFloatConfigChecked(FName Name);
};

inline float UGlobalConfigLibrary::ReadFloatConfigChecked(FName Name)
{
	FOptionalFloat Res = ReadFloatConfig(Name);
	ensure(Res.Valid);
	return Res.Value;
}