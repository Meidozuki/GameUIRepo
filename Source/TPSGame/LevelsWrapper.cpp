// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelsWrapper.h"

void ALevelsWrapper::SeamlessOpenLevel2(const UObject* WorldContextObject, FName LevelName)
{
	WorldContextObject->GetWorld()->SeamlessTravel(LevelName.ToString());
}

void ALevelsWrapper::SeamlessOpenLevel(FString LevelName)
{
	GetWorld()->SeamlessTravel(LevelName);
}
