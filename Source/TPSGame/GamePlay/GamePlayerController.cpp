// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayerController.h"

#include "CameraController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "QuickLog.h"

PRAGMA_DISABLE_OPTIMIZATION

AGamePlayerController::AGamePlayerController()
{
	;
}

void AGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	LOGDEBUG(TEXT("SetupInputComponent called"));
}