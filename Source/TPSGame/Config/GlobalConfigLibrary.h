// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GlobalConfigTypes.h"
#include "GlobalConfigLibrary.generated.h"



UCLASS()
class TPSGAME_API UGlobalConfigLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
public:
	UFUNCTION(BlueprintPure)
	static FOptionalFloat ReadFloatConfig(FName Name);
	
	UFUNCTION(BlueprintCallable)
	static inline float ReadFloatConfigChecked(FName Name);
};

float UGlobalConfigLibrary::ReadFloatConfigChecked(FName Name)
{
	FOptionalFloat Res = ReadFloatConfig(Name);
	ensure(Res.Valid);
	return Res.Value;
}