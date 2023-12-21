// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LevelsWrapper.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class TPSGAME_API ALevelsWrapper : public AActor
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OpenLevel(const UObject* WorldContextObject, FName LevelName);

	UFUNCTION(BlueprintCallable)
	static void SeamlessOpenLevel2(const UObject* WorldContextObject, FName LevelName);
	
	UFUNCTION(BlueprintCallable)
	void SeamlessOpenLevel(FString LevelName);
};
