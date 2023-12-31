// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Board.generated.h"


class ABasicGrid;
class UBoard2DComponent;

/**
 * All public function return XY plane
 */
UCLASS(Blueprintable)
class TPSGAME_API ABoard : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* GridMesh = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<AActor> GridType = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UBoard2DComponent* Board2D;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ABoard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	
};
