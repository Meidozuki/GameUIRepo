// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Materials/MaterialInstance.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "MaterialInstanceUtil.generated.h"


USTRUCT()
struct FMaterialInstanceUpdateInfo
{
	GENERATED_BODY()
public:
	bool bVerbose = false;
	TArray<FScalarParameterValue> ScalarParameterValues;
	// TArray<FVectorParameterValue> VectorParameterValues;

	FMaterialInstanceUpdateInfo() = default;
	FMaterialInstanceUpdateInfo(UMaterialInterface* MaterialInterface)
	{
		if (auto *MI = Cast<UMaterialInstance>(MaterialInterface))
		{
			ScalarParameterValues = MI->ScalarParameterValues;
		}
	}

	void PurgeParameters(UMaterialInterface* OriginalMaterial, bool bOverridenOnly=true, bool bCurrentOnly=false);
};

USTRUCT()
struct FMaterialParameterHelper
{
	GENERATED_BODY()

	static void ViewScalarParameters(TArray<FScalarParameterValue> const& ScalarParameterValues);
	static void ViewScalarParameters(UMaterialInstance* MaterialInstance);
	
	// Find whether current material (instance) have the parameter named ParamName
	static bool FindParameterByNameGeneral(UMaterialInterface* MaterialInterface, const FName& ParamName);
};

// Used for replacing newly created UMaterialInstanceDynamic's parameters
UCLASS()
class UHotReloadMIDProxy final: public UObject
{
	GENERATED_BODY()
public:
	static void Insert(UMaterialInterface* Key, const FMaterialInstanceUpdateInfo& Value);

	static void UpdateMID(UMaterialInstanceDynamic* MID);

	static void RegisterMIDCreateDelegate();
private:
	static bool bActivate;
	static TMap<UMaterialInterface*, FMaterialInstanceUpdateInfo> Mapping;
};


UCLASS()
class UMaterialInstanceUtil : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintCallable)
	static void ViewScalarParameters(UMaterialInstance* MaterialInstance)
	{
		FMaterialParameterHelper::ViewScalarParameters(MaterialInstance);
	}
	
	static TArray<UMaterialInstanceDynamic*> CollectMaterialInstanceDynamics();

	/** Default: Find the same MIs, or MIDs from Base
	 * 仅会返回当前可以处理的情况
	 * 现在为，相同MIC或者MIC派生的MID
	 */
	UFUNCTION(BlueprintCallable)
	static bool IsMaterialInstanceDerivedFrom(UMaterialInterface* Derived, UMaterialInterface* Base, bool bAffectChildren = false);
	
	
	UFUNCTION(BlueprintCallable)
	static void TestBatchUpdateMID(FName Name, float Value, UMaterialInterface* ParentConstraint)
	{
		FScalarParameterValue Info;
		Info.ParameterInfo = Name;
		Info.ParameterValue = Value;
		FMaterialInstanceUpdateInfo UpdateInfo;
		UpdateInfo.ScalarParameterValues.Add(Info);
		BatchUpdateMIDScalarParam(CollectMaterialInstanceDynamics(), UpdateInfo, ParentConstraint);
	}
	
	// UFUNCTION(BlueprintCallable)
	// static bool FindParameterByNameGeneral(UMaterialInterface* MaterialInterface, const FName& ParamName)
	// {
	// 	return FMaterialParameterHelper::FindParameterByNameGeneral(MaterialInterface, ParamName);
	// }

	
	UFUNCTION(BlueprintCallable)
	static bool IsMIDDerivedFrom(UMaterialInterface* Derived, UMaterialInterface* Base);
	
	static void UpdateMIDScalarParam(UMaterialInstanceDynamic* MID, FName ParamName, float NewValue);
	
	static void UpdateMIDScalarParam(UMaterialInstanceDynamic* MID,
									const FMaterialInstanceUpdateInfo& UpdateInfo,
									UMaterialInterface* ParentConstraint = nullptr);
	
	static void BatchUpdateMIDScalarParam(const TArray<UMaterialInstanceDynamic*> &Array,
										const FMaterialInstanceUpdateInfo& UpdateInfo,
										UMaterialInterface* ParentConstraint = nullptr);
	
	/**
	 * Returns true if modifying MITop's ParamName can affect MIBottom
	 */
	UFUNCTION(BlueprintCallable)
	static bool IsParameterInheritedFrom(const FName& ParamName, UMaterialInterface* MIBottom, UMaterialInterface* MITop);
};
