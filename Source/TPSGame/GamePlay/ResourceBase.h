// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAmountChanged, float, NewAmount);

UCLASS(BlueprintType)
class TPSGAME_API AResourceBase : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	AResourceBase() = default;

	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintAssignable)
	FOnAmountChanged OnAmountChanged;
	
	UPROPERTY(BlueprintReadOnly)
	float Amount = 100;

	UPROPERTY(BlueprintReadOnly)
	float DecreaseVelocity = 5;

	UFUNCTION()
	void ResourceTick();
};
