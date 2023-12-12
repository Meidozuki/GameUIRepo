// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "WorldCamera.generated.h"

UCLASS()
class TPSGAME_API AWorldCamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AWorldCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	UInputAction* CameraMoveAction;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = Input)
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Input)
	FVector CameraMoveVelocity = FVector::OneVector;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void CameraMove(const FInputActionValue& Value);
};
