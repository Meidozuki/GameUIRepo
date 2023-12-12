// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "InputMappingContext.h"
#include "GamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TPSGAME_API AGamePlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	AGamePlayerController();

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* InputMappingContext;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
};
