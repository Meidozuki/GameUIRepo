// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialInstanceUtil.h"

#include "EngineUtils.h"
#include "Components/MeshComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Misc/CoreMiscDefines.h"

bool UHotReloadMIDProxy::bActivate = false;
TMap<UMaterialInterface*, FMaterialInstanceUpdateInfo> UHotReloadMIDProxy::Mapping;

// PRAGMA_DISABLE_OPTIMIZATION

void FMaterialInstanceUpdateInfo::PurgeParameters(UMaterialInterface* OriginalMaterial, bool bOverridenOnly, bool bCurrentOnly)
{
	if (!IsValid(OriginalMaterial))
	{
		return;
	}
	// If an MID is created from a UMaterial, all params is considered mutable
	// pass
	
	if (OriginalMaterial->IsA<UMaterialInstance>())
	{
		if (bVerbose)
		{
			UE_LOG(LogTemp, Display, TEXT("Before parameter purge:"));
			FMaterialParameterHelper::ViewScalarParameters(ScalarParameterValues);
		}
		auto *MI = Cast<UMaterialInstance>(OriginalMaterial);
		TArray<int> DeleteIdx;
		for (int i=0;i < ScalarParameterValues.Num();++i)
		{
			auto &Ref = ScalarParameterValues[i];
			if (bCurrentOnly)
			{
				if (!FMaterialParameterHelper::FindParameterByNameGeneral(OriginalMaterial, Ref.ParameterInfo.Name))
				{
					DeleteIdx.Insert(i, 0);
				}
			}
			else if (bOverridenOnly)
			{
				float Value = NAN;
				if (!MI->GetScalarParameterValue(Ref.ParameterInfo, Value, bOverridenOnly))
				{
					DeleteIdx.Insert(i, 0);
				}
			}
		}

		// Reverse order delete
		for (int Idx: DeleteIdx)
		{
			ScalarParameterValues.RemoveAtSwap(Idx);
		}
		
		if (bVerbose)
		{
			UE_LOG(LogTemp, Display, TEXT("After parameter purge:"));
			FMaterialParameterHelper::ViewScalarParameters(ScalarParameterValues);
		}
	}
}

void FMaterialParameterHelper::ViewScalarParameters(TArray<FScalarParameterValue> const& ScalarParameterValues)
{
	for (auto &Param: ScalarParameterValues)
	{
		UE_LOG(LogTemp, Display, TEXT("[FMaterialParameterHelper]Scalar params: %s %f"), *Param.ParameterInfo.Name.ToString(), Param.ParameterValue);
	}
	UE_LOG(LogTemp, Display, TEXT("[FMaterialParameterHelper]Scalar params array end."));
}

void FMaterialParameterHelper::ViewScalarParameters(UMaterialInstance* MaterialInstance)
{
	if (IsValid(MaterialInstance))
	{
		ViewScalarParameters(MaterialInstance->ScalarParameterValues);
	}
}

template <typename ParameterType>
const ParameterType* FindParameterByName(const TArray<ParameterType>& Parameters, const FHashedMaterialParameterInfo& ParameterInfo)
{
	// Copied from engine
	for (int32 ParameterIndex = 0; ParameterIndex < Parameters.Num(); ParameterIndex++)
	{
		const ParameterType* Parameter = &Parameters[ParameterIndex];
		if (Parameter->ParameterInfo == ParameterInfo)
		{
			return Parameter;
		}
	}
	return nullptr;
}

bool FMaterialParameterHelper::FindParameterByNameGeneral(UMaterialInterface* MaterialInterface, const FName& ParamName)
{
	if (!IsValid(MaterialInterface))
	{
		return false;
	}

	if (MaterialInterface->IsA<UMaterialInstance>())
	{
		auto *MaterialInstance = Cast<UMaterialInstance>(MaterialInterface);
		return FindParameterByName(MaterialInstance->ScalarParameterValues, ParamName) != nullptr;
	}
	else
	{
		auto *Material = Cast<UMaterial>(MaterialInterface);
		float temp;
		return Material->GetScalarParameterValue(ParamName, temp, false);
	}
}

void UHotReloadMIDProxy::Insert(UMaterialInterface* Key, const FMaterialInstanceUpdateInfo& Value)
{
	if (IsValid(Key) && Mapping.Find(Key) == nullptr && !Key->IsA<UMaterialInstanceDynamic>())
	{
		Mapping.Add(Key, Value);
		RegisterMIDCreateDelegate();
	}
}

void UHotReloadMIDProxy::UpdateMID(UMaterialInstanceDynamic* MID)
{
	if (!IsValid(MID))
	{
		return;
	}
	auto *Value = Mapping.Find(MID->Parent);
	if (Value != nullptr)
	{
		UMaterialInstanceUtil::UpdateMIDScalarParam(MID, *Value);
	}
}

void UHotReloadMIDProxy::RegisterMIDCreateDelegate()
{
	// if (!bActivate)
	// {
	// 	UMaterialInstanceDynamic::PostMIDCreate.AddStatic(&USGHotReloadMIDProxy::UpdateMID);
	// 	bActivate = true;
	// }
}

TArray<UMaterialInstanceDynamic*> UMaterialInstanceUtil::CollectMaterialInstanceDynamics()
{
	TArray<UMaterialInstanceDynamic*> Res;
	for (TObjectIterator<UMaterialInstanceDynamic> It; It; ++It)
	{
		Res.Add(*It);
	}
	return Res;
}

bool UMaterialInstanceUtil::IsMaterialInstanceDerivedFrom(UMaterialInterface* Derived, UMaterialInterface* Base,
                                                          bool bAffectChildren)
{
	constexpr bool verbose = true;
	if (Base == nullptr || Derived == nullptr)
	{
		return false;
	}
	UE_LOG(LogTemp, Display, TEXT("Material derive judging %s and %s"), *Derived->GetName(), *Base->GetName());
	if (Derived == Base)
	{
		return Derived->IsA<UMaterialInstance>();
	}

	if (!bAffectChildren)
	{
		//都是Constant和Dynamic的情况上面包括了
		if (auto *MID = Cast<UMaterialInstanceDynamic>(Derived))
		{
			if (MID->Parent == Base)
			{
				return true;
			}
		}
		return false;
	}

	bool flag = false;
	auto *MI = Cast<UMaterialInstance>(Derived);
#if !UE_BUILD_SHIPPING
	TArray<FString> DebugStrings;
	while (MI != nullptr)
	{
		if (MI->Parent == Base)
		{
			flag=true;
			DebugStrings.Add(MI->GetName());
		}
		MI = Cast<UMaterialInstance>(MI->Parent);
	}

	UE_LOG(LogTemp, Log, TEXT("MI tree: %s"), *FString::Join(DebugStrings, TEXT(" -- ")));
#else
	while (MI != nullptr)
	{
		if (MI->Parent == Base)
		{
			flag=true;
			break;
		}
		MI = Cast<UMaterialInstance>(MI->Parent);
	}
#endif
	return flag;
}

bool UMaterialInstanceUtil::IsMIDDerivedFrom(UMaterialInterface* Derived, UMaterialInterface* Base)
{
	if (!IsValid(Derived) || !IsValid(Base))
	{
		return false;
	}
	if (!Derived->IsA<UMaterialInstanceDynamic>())
	{
		return false;
	}

	auto *MI = Derived;
	while (MI->IsA<UMaterialInstance>())
	{
		if (MI == Base)
		{
			return true;
		}
		MI = Cast<UMaterialInstance>(MI)->Parent;
	}
	return MI == Base;
}

// bool UMaterialInstanceUtil::UpdateMIScalarParam(UMeshComponent* Mesh, int MaterialIndex, FName ParamName, float NewValue, bool bOveriddenOnly)
// {
// 	if (!IsValid(Mesh))
// 		return false;
//
// 	if (MaterialIndex < 0 || MaterialIndex >= Mesh->GetNumMaterials())
// 		return false;
//
// 	auto *Material = Mesh->GetMaterial(MaterialIndex);
// 	float OldValue;
// 	if (!Material->GetScalarParameterValue(ParamName, OldValue, bOveriddenOnly))
// 	{
// 		return false;
// 	}
// 	
// 	const UClass* Class = Material->GetClass();
// 	if (Class == UMaterialInstanceConstant::StaticClass())
// 	{
// 		auto *MID = UMaterialInstanceDynamic::Create(
// 			Cast<UMaterialInstance>(Material), GetTransientPackage());
// 		MID->SetScalarParameterValue(ParamName, NewValue);
// 		Mesh->SetMaterial(MaterialIndex, MID);
//
// #if !UE_BUILD_SHIPPING
// 		UE_LOG(LogTemp, Display, TEXT("[] MaterialInstance param changed from %f to %f"), OldValue, NewValue);
// #endif
// 		return true;
// 	}
// 	else if (Class == UMaterialInstanceDynamic::StaticClass())
// 	{
// 		auto *MID = Cast<UMaterialInstanceDynamic>(Material);
// 		MID->SetScalarParameterValue(ParamName, NewValue);
// #if !UE_BUILD_SHIPPING
// 		UE_LOG(LogTemp, Display, TEXT("[" TEXT(__FILE__) "] MaterialInstance param changed from %f to %f"),  OldValue, NewValue);
// #endif
// 		return true;
// 	}
// 	else
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("You are trying to directly change a UMaterial's parameter"));
// 		return false;
// 	}
// 	
// }

void UMaterialInstanceUtil::UpdateMIDScalarParam(UMaterialInstanceDynamic* MID, FName ParamName, float NewValue)
{
	MID->SetScalarParameterValue(ParamName, NewValue);
}

void UMaterialInstanceUtil::UpdateMIDScalarParam(UMaterialInstanceDynamic* MID,
												  const FMaterialInstanceUpdateInfo& UpdateInfo,
												  UMaterialInterface* ParentConstraint)
{
	const bool bConstraintValid = ParentConstraint != nullptr;
	for (auto &ScalarParam: UpdateInfo.ScalarParameterValues)
	{
		auto &ParamName = ScalarParam.ParameterInfo.Name;
		// 跳过Parameter
		if (bConstraintValid && !IsParameterInheritedFrom(ParamName, MID, ParentConstraint))
		{
			continue;
		}
		MID->SetScalarParameterValue(ParamName, ScalarParam.ParameterValue);
	}
}

void UMaterialInstanceUtil::BatchUpdateMIDScalarParam(const TArray<UMaterialInstanceDynamic*> &Array,
                                                      const FMaterialInstanceUpdateInfo& UpdateInfo,
                                                      UMaterialInterface* ParentConstraint)
{
	const bool bConstraintValid = IsValid(ParentConstraint);
	for (auto *MID: Array)
	{
		if (!IsValid(MID))
		{
			continue; //check pending kill
		}

		if (!bConstraintValid)
		{
			UpdateMIDScalarParam(MID, UpdateInfo, nullptr);
		}
		else
		{
			// 跳过MID
			if (!IsMIDDerivedFrom(MID, ParentConstraint))
			{
				continue;
			}
			UpdateMIDScalarParam(MID, UpdateInfo, ParentConstraint);
		}
	}
}


bool UMaterialInstanceUtil::IsParameterInheritedFrom(const FName& ParamName, UMaterialInterface* MIBottom, UMaterialInterface* MITop)
{
	if (!IsValid(MIBottom) || !IsValid(MITop))
	{
		return false;
	}

	float temp;
	if (MIBottom->IsA<UMaterial>())
	{
		// 如果Bottom就是材质，可以短路判断
		return MIBottom == MITop && MIBottom->GetScalarParameterValue(ParamName, temp);
	}
	if (MIBottom == MITop)
	{
		// 如果修改的就是自身，只需要含有该参数
		return MIBottom->GetScalarParameterValue(ParamName, temp);
	}

	bool flag = true;
	for (auto *MI = Cast<UMaterialInstance>(MIBottom);
		MI != nullptr && MI != MITop; )
	{
		// 如果Bottom到Top中间有其他值覆盖，则Top不会影响到Bottom
		bool found = FMaterialParameterHelper::FindParameterByNameGeneral(MI, ParamName);
#if WITH_DEV_AUTOMATION_TESTS
		UE_LOG(LogTemp, Log, TEXT("%s found param %s: %d"), *MI->GetName(), *ParamName.ToString(), int(found));
#endif
		if (found)
		{
			flag = false;
			break;
		}

		MI = Cast<UMaterialInstance>(MI->Parent);
	}

	return flag && IsMIDDerivedFrom(MIBottom, MITop) && MITop->GetScalarParameterValue(ParamName, temp);
}



// void UMaterialInstanceUtil::UpdateMIScalarParamBySlot(UMeshComponent* Mesh, FName SlotName, FName ParamName,
//                                                       float NewValue)
// {
// 	int32 Index = Mesh->GetMaterialIndex(SlotName);
// 	if (Index != INDEX_NONE)
// 	{
// 		UpdateMIScalarParam(Mesh, Index, ParamName, NewValue);
// 	}
// }

// void UMaterialInstanceUtil::UpdateAllMIScalarParam(UWorld* InWorld, UMaterialInstance* Target, FName ParamName,
// 	float NewValue, bool bAffectChildren)
// {
// 	FMaterialInstanceUpdateInfo Info;
// 	auto &Param = Info.ScalarParameterValues.AddDefaulted_GetRef();
// 	Param.ParameterInfo = ParamName;
// 	Param.ParameterValue = NewValue;
// 	BatchUpdateMIScalarParam(InWorld, Target, Info, AActor::StaticClass(), bAffectChildren);
// }
//
// void UMaterialInstanceUtil::BatchUpdateMIScalarParam(UWorld* InWorld, UMaterialInstance* Target,
// 	const FMaterialInstanceUpdateInfo& UpdateInfo, TSubclassOf<AActor> ActorType, bool bAffectChildren)
// {
// 	if (IsValid(Target) && !Target->IsA<UMaterialInstance>())
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("[%s] Try to update MaterialInstance's param, but ptr is invalid or is not an instance"));
// 		return;
// 	}
// 	
// 	auto ComponentUpdateParam = [Target, &UpdateInfo, bAffectChildren](UMeshComponent* Component)
// 	{
// 		for (int i=0; i < Component->GetNumMaterials(); ++i)
// 		{
// 			// 如果是选择的子材质实例
// 			if (IsMaterialInstanceDerivedFrom(Component->GetMaterial(i), Target, bAffectChildren))
// 			{
// 				// 则更新参数
// 				for (auto &ScalarParam: UpdateInfo.ScalarParameterValues)
// 				{
// 					bool bSuccess = UpdateMIScalarParam(Component, i, ScalarParam.ParameterInfo.Name, ScalarParam.ParameterValue);
// 				}
// 			}
// 		}
// 	};
//
// 	// 遍历所有ActorType类型的Actor
// 	for (TActorIterator<AActor> It(InWorld, ActorType); It; ++It)
// 	{
// 		//检查所有MeshComponent
// 		It->ForEachComponent<UMeshComponent>(true, ComponentUpdateParam);
// 	}
// }