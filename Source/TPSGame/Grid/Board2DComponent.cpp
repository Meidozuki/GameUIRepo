// Fill out your copyright notice in the Description page of Project Settings.


#include "Board2DComponent.h"

#include "QuickLog.h"
#include "UtilFuncLibrary.h"
#include "Config/GlobalConfigLibrary.h"


UBoard2DComponent::UBoard2DComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

inline float ReadGridSize(FName Name)
{
	float temp = UGlobalConfigLibrary::ReadFloatConfigChecked(Name);
	return temp != 0.f ? temp : 1.f;
}

void UBoard2DComponent::Init()
{
	float GridX = ReadGridSize(TEXT("GridSizeX"));
	float GridY = ReadGridSize(TEXT("GridSizeY"));
	GridSize = { GridX, GridY };
	
	auto&& Location = UUtilityFunctionLibrary::RotateScreenToXYSpace(GetRelativeLocation());
	LOGDEBUG(TEXT("Board2D location %s"), *Location.ToString());
	FVector2f XY(Location.X, Location.Y);

	GridBegin = XY - GridRange / 2;
	GridEnd = XY + GridRange / 2;
}

void UBoard2DComponent::ForEachGrid(TFunction<void(float, float)> Func)
{
	float EndX = GridEnd.X, EndY = GridEnd.Y;
	for (float Y = GridBegin.Y; Y < EndY; Y += GridSize.Y) {
		for (float X = GridBegin.X; X < EndX; X += GridSize.X) {
			Func(Y, X);
		}
	}
}

// Called when the game starts
void UBoard2DComponent::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Display, TEXT("Board2D comp begin play, owner is %s"), *GetOwner()->GetName());

	Init();
	
}

bool UBoard2DComponent::IsInBoard(FVector2 InLocation) const
{
	return InLocation.ComponentwiseAllGreaterThan(GridBegin) &&\
		InLocation.ComponentwiseAllLessThan(GridEnd);
}

UBoard2DComponent::FVector2 UBoard2DComponent::AlignGridLocation(FVector2 InLocation) const
{
	float GridX = GridSize.X, GridY = GridSize.Y;
	
	// Use RoundTo other than SnapGrid to enable Unreal's SSE
	return FVector2f(
		FMath::RoundToInt(InLocation.X / GridX) * GridX,
		FMath::RoundToInt(InLocation.Y / GridY) * GridY
	);
}



