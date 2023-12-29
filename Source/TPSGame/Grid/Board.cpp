// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

#include "BasicGrid.h"
#include "Math/UnrealMathUtility.h"

#include "Config/GlobalConfigLibrary.h"


// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	GridRange = {400, 400};
}

inline float ReadGridSize(FName Name)
{
	float temp = UGlobalConfigLibrary::ReadFloatConfigChecked(Name);
	return temp != 0.f ? temp : 1.f;
}

FVector2f ABoard::GetGridSize()
{
	static float GridX = ReadGridSize(TEXT("GridSizeX"));
	static float GridY = ReadGridSize(TEXT("GridSizeY"));
	return {GridX, GridY};
}

// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();

	FVector Extent = Box->GetUnscaledBoxExtent();
	FVector2f Ratio = GridRange / FVector2f(Extent.X, Extent.Y);
	SetActorScale3D(FVector(Ratio.X, Ratio.Y, 1.0f));
	const auto X = GetActorLocation().X;

	for (float i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			auto *Actor = GetWorld()->SpawnActor<ABasicGrid>(FVector(X, j, i), {});
			Actor->StaticMeshComponent->SetStaticMesh(GridMesh);
		}
	}
}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABoard::IsInBoard(FVector InLocation) const
{
	auto &&Location = GetActorLocation();
	FVector2f XY(Location.X, Location.Y);
	
	auto &&Half = GridRange / 2;
	const auto RangeBegin = XY - Half;
	const auto RangeEnd = XY + Half;
	
	return XY.ComponentwiseAllGreaterThan(RangeBegin) && XY.ComponentwiseAllLessThan(RangeEnd);
}


FVector ABoard::AlignGridLocation(FVector InLocation) const
{
	auto temp = GetGridSize();
	float GridX = temp.X, GridY = temp.Y;
	
	// Use RoundTo other than SnapGrid to enable Unreal's SSE
	return FVector(
		FMath::RoundToInt(InLocation.X / GridX) * GridX,
		FMath::RoundToInt(InLocation.Y / GridY) * GridY,
		InLocation.Z
	);
}

