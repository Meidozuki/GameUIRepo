// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "BasicGrid.generated.h"


UCLASS(Blueprintable)
class TPSGAME_API ABasicGrid : public AActor
{
	GENERATED_BODY()

public:
	ABasicGrid();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UFUNCTION(BlueprintNativeEvent)
	void AdaptiveResize(FVector2f GridSize);
};
