// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MaterialRefactor.generated.h"


// MaterialInstanceDynamic.h

// DECLARE_MULTICAST_DELEGATE_OneParam(FPostMaterialInstanceDynamicCreateSignature, UMaterialInstanceDynamic*);
// UCLASS(hidecategories=Object, collapsecategories, BlueprintType)
// class ENGINE_API UMaterialInstanceDynamic : public UMaterialInstance{
// 	GENERATED_UCLASS_BODY()
//
// 	/**
// 	 * Create a material instance dynamic parented to the specified material.
// 	 */
// 	static UMaterialInstanceDynamic* Create(class UMaterialInterface* ParentMaterial, class UObject* InOuter);
// 	
// 	static FPostMaterialInstanceDynamicCreateSignature PostMIDCreate;
//
// 	// ...
// };




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
	
	// UFUNCTION(BlueprintCallable)
	static void ForceUpdateMIParam(UMaterialInstance* MI, FName ParamName, float NewValue);

	UFUNCTION(BlueprintCallable)
	static void CopyParamater(const UStaticMeshComponent* From, UStaticMeshComponent* To);
	
	
};
