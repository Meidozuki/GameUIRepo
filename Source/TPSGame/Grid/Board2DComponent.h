// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Math/MathFwd.h"
#include "Board2DComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TPSGAME_API UBoard2DComponent : public USceneComponent
{
	GENERATED_BODY()
public:
	using FVector2D = FVector2f;

	// uproprty和ufunction无法使用using
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	FVector2D GridRange;
	
private:
	FVector2D GridSize;
	FVector2D GridBegin, GridEnd;

	void Init();

	// inline float GetGridSizeX() const { return GridSize.X; }
	// inline float GetGridSizeY() const { return GridSize.Y; }

	void ForEachGrid(TFunction<void(float, float)> Func);
	
public:
	// Sets default values for this component's properties
	UBoard2DComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//                            FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	bool IsInBoard(FVector2D InLocation) const;
	
	UFUNCTION(BlueprintCallable)
	FVector2D AlignGridLocation(FVector2D InLocation) const;
	
};
