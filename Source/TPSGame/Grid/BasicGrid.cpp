// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicGrid.h"


ABasicGrid::ABasicGrid()
{
	PrimaryActorTick.bCanEverTick = true;
	AlignPosition = 0.f;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ABasicGrid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

