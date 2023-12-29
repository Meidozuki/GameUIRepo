// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseCollisionCheck.h"


// Sets default values
AMouseCollisionCheck::AMouseCollisionCheck()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMouseCollisionCheck::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMouseCollisionCheck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AMouseCollisionCheck::GetMousePosition()
{
	FHitResult Hit;
	bool bHit = GetWorld()->GetFirstPlayerController()->\
		GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	if (bHit)
	{
		DrawDebugPoint(GetWorld(), Hit.Location, 50, FColor::Red);
	}
	return bHit ? Hit.Location : FVector();
}

FVector AMouseCollisionCheck::GetMousePosition2()
{
	auto *Controller = GetWorld()->GetFirstPlayerController();
	FVector Pos, Dir;
	if (Controller->DeprojectMousePositionToWorld(Pos, Dir))
	{
		FPlane Plane({0,0,0},{0,0,1});
		FVector HitLocation = FMath::RayPlaneIntersection(Pos, Dir, Plane);
		// todo:check location in screen
		return HitLocation;
	}
	else
		return {};
}

