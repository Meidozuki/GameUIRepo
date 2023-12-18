// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialRefactor.generated.h"

UCLASS()
class AMaterialRefactor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMaterialRefactor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable)
	static void ForceUpdateMIParam(UMaterialInstance* MI, FName ParamName, float NewValue);
	
	UFUNCTION(BlueprintCallable)
	void UpdateMaterialSlotScalarParamater(FName InSlotName, FName InParamName, float ScalarVal);

	UFUNCTION(BlueprintCallable)
	static void CopyParamater(const UStaticMeshComponent* From, UStaticMeshComponent* To);
	
	UFUNCTION(BlueprintCallable)
	void FindMaterialChild(UMaterialInterface *MaterialInterface);
	
};
