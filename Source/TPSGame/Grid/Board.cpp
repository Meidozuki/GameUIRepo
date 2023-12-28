// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

#include "Math/UnrealMathUtility.h"

#include "Config/GlobalConfigLibrary.h"


// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABoard::IsInBoard(FVector InLocation) const
{
	
	return true;
}

inline float ReadGridSize(FName Name)
{
	float temp = UGlobalConfigLibrary::ReadFloatConfigChecked(Name);
	return temp != 0.f ? temp : 1.f;
}

FVector ABoard::AlignGridLocation(FVector InLocation) const
{
	static float GridX = ReadGridSize(TEXT("GridSizeX"));
	static float GridY = ReadGridSize(TEXT("GridSizeY"));
	
	// Use RoundTo other than SnapGrid to enable Unreal's SSE
	return FVector(
		FMath::RoundToInt(InLocation.X / GridX) * GridX,
		FMath::RoundToInt(InLocation.Y / GridY) * GridY,
		InLocation.Z
	);
}

