// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceBase.h"

void AResourceBase::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = false;

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle, this, &AResourceBase::ResourceTick, 1.0f, true
		);
}

void AResourceBase::ResourceTick()
{
	UE_LOG(LogTemp, Display, TEXT("Amount: %f"), Amount);
	Amount -= DecreaseVelocity;
	OnAmountChanged.Broadcast(Amount);
}
