// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialRefactor.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/PropertyAccessUtil.h"


// MaterialInstanceDynamic.cpp

// FPostMaterialInstanceDynamicCreateSignature UMaterialInstanceDynamic::PostMIDCreate;
//
//
// UMaterialInstanceDynamic* UMaterialInstanceDynamic::Create(UMaterialInterface* ParentMaterial, UObject* InOuter)
// {
// 	LLM_SCOPE(ELLMTag::MaterialInstance);
// 	UObject* Outer = InOuter ? InOuter : GetTransientPackage();
// 	UMaterialInstanceDynamic* MID = NewObject<UMaterialInstanceDynamic>(Outer);
// 	MID->SetParentInternal(ParentMaterial, false);
//
// 	PostMIDCreate.Broadcast(MID);
// 	return MID;
// }





// Sets default values
AMaterialRefactor::AMaterialRefactor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AMaterialRefactor::CopyParamater(const UStaticMeshComponent* From, UStaticMeshComponent* To)
{
	UMaterialInterface *Material = From->GetMaterial(0);
	// if (auto *MI = Cast<UMaterialInstance>(Material))
	// {
	// 	auto *MID = UMaterialInstanceDynamic::Create(Material, GetTransientPackage());
	// 	MID->CopyMaterialUniformParameters(MI);
	// 	To->SetMaterial(0, MID);
	// 	FVector Res;
	// 	MID->GetScalarParameterValue(TEXT("R"), Res.X);
	// 	MID->GetScalarParameterValue(TEXT("G"), Res.Y);
	// 	MID->GetScalarParameterValue(TEXT("B"), Res.Z);
	// 	UE_LOG(LogTemp,Warning,TEXT("%s"), *Res.ToString());
	// }
	
	if (auto *MI = Cast<UMaterialInstanceConstant>(Material))
	{
		auto *MID = UMaterialInstanceDynamic::Create(Material, GetTransientPackage());
		To->SetMaterial(0, MID);
	}
}



// Called when the game starts or when spawned
void AMaterialRefactor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMaterialRefactor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMaterialRefactor::ForceUpdateMIParam(UMaterialInstance* MI, FName ParamName, float NewValue)
{
	if (IsValid(MI))
	{
		static_cast<UMaterialInstanceDynamic*>(MI)->SetScalarParameterValue(ParamName, NewValue);

		MI->MarkPackageDirty();
	}
}
