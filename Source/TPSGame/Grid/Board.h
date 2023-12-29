// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Board.generated.h"

UCLASS(Blueprintable)
class TPSGAME_API ABoard : public AActor
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	FVector2f GridRange;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* GridMesh = nullptr;
private:
	static FVector2f GetGridSize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ABoard();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsInBoard(FVector InLocation) const;
	
	UFUNCTION(BlueprintCallable)
	FVector AlignGridLocation(FVector InLocation) const;

	
};
