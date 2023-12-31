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
	using FVector2 = FVector2f;

	// uproprty和ufunction无法使用using
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.01"))
	FVector2f GridRange;
	
private:
	FVector2 GridSize;
	FVector2 GridBegin, GridEnd;

	void Init();

	// inline float GetGridSizeX() const { return GridSize.X; }
	// inline float GetGridSizeY() const { return GridSize.Y; }
	
public:
	// Sets default values for this component's properties
	UBoard2DComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	//                            FActorComponentTickFunction* ThisTickFunction) override;

	const FVector2& GetGridSize() const { return GridSize;}

	UFUNCTION(BlueprintCallable)
	void flush() { Init();}

	void ForEachGrid(TFunction<void(float, float)> Func);
	
	//UFUNCTION(BlueprintCallable)
	bool IsInBoard(FVector2 InLocation) const;
	
	//UFUNCTION(BlueprintCallable)
	FVector2 AlignGridLocation(FVector2 InLocation) const;
	
};
