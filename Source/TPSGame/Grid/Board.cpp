// Fill out your copyright notice in the Description page of Project Settings.


#include "Board.h"

#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"

#include "BasicGrid.h"
#include "QuickLog.h"
#include "Board2DComponent.h"
#include "UtilFuncLibrary.h"

// Sets default values
ABoard::ABoard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Board2D = CreateDefaultSubobject<UBoard2DComponent>("Board2D");
	Board2D->GridRange = {400, 400};
}


// Called when the game starts or when spawned
void ABoard::BeginPlay()
{
	Super::BeginPlay();
	
	UE_LOG(LogTemp, Display,TEXT("Board actor begin play"));

	FVector Extent = Box->GetUnscaledBoxExtent();
	FVector2f Ratio = Board2D->GridRange / FVector2f(Extent.X, Extent.Y);
	SetActorScale3D(UUtilityFunctionLibrary::RotateXYToScreenSpace({ Ratio.X, Ratio.Y, 0.5f }));

	FVector Location = GetActorLocation();
	LOGDEBUG(TEXT("Draw At %s, Extent*2 = %s"), *Location.ToString(), *Box->GetScaledBoxExtent().ToString());
	DrawDebugBox(GetWorld(), Location, Box->GetScaledBoxExtent() / 2, FColor::Red, true);


	if (IsValid(GridType))
	{
		LOGDEBUG(TEXT("Grid Type is %s"), *GridType->GetName());
		Board2D->ForEachGrid([Location, this](float i, float j) {
			FVector Translation = FVector(Location.X, j, i) + Location;
			auto* Actor = this->GetWorld()->SpawnActor(*GridType, &Translation);
			if (auto *Grid = Cast<ABasicGrid>(Actor))
			{
				Grid->AdaptiveResize(FVector2f(Board2D->GetGridSize()));
			}
			});
	}

}

// Called every frame
void ABoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

