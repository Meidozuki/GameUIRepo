// Fill out your copyright notice in the Description page of Project Settings.


#include "MaterialRefactor.h"
#include "EngineUtils.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstance.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/PropertyAccessUtil.h"

// Sets default values
AMaterialRefactor::AMaterialRefactor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

void AMaterialRefactor::UpdateMaterialSlotScalarParamater(FName InSlotName, FName InParamName, float ScalarVal)
{
	for (TActorIterator<ACharacter> It(GetWorld()); It; ++It)
	{
		auto *SkeletalMeshComponent = Cast<USkeletalMeshComponent>(It->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
		FString Name = It->GetName();
		// UE_LOG(LogTemp, Display, TEXT("%s"), *Name);
		if (!IsValid(SkeletalMeshComponent))
		{
			continue;
		}
		
		int32 Index = SkeletalMeshComponent->GetMaterialIndex(InSlotName);
		if (Index == INDEX_NONE)
		{
			continue;
		}
		
		if (auto *MaterialInstance = Cast<UMaterialInstance>(SkeletalMeshComponent->GetMaterial(Index)))
		{
			auto *MID = Cast<UMaterialInstanceDynamic>(MaterialInstance);
			float Current;
			if (MID)
			{
				MID->GetScalarParameterValue(InParamName, Current);
				MID->SetScalarParameterValue(InParamName, ScalarVal);
				SkeletalMeshComponent->MarkRenderDynamicDataDirty();
			}
			else
			{
				MID = UMaterialInstanceDynamic::Create(MaterialInstance, GetTransientPackage());
				MID->CopyParameterOverrides(MaterialInstance);
			
				MID->GetScalarParameterValue(InParamName, Current);
				MID->SetScalarParameterValue(InParamName, ScalarVal);
			
				SkeletalMeshComponent->SetMaterial(Index, MID);
			}
			constexpr int MessageID = 100;
			GEngine->AddOnScreenDebugMessage(MessageID,1,FColor::Red, FString::Printf(
					TEXT("%s changed from %f to %f"), *InParamName.ToString(), Current, ScalarVal));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Trying to modify %s through material not a material instance"), *InSlotName.ToString());
			return;
		}
		
	}

}

void AMaterialRefactor::CopyParamater(const UStaticMeshComponent* From, UStaticMeshComponent* To)
{
	UMaterialInterface *Material = From->GetMaterial(0);
	if (auto *MI = Cast<UMaterialInstance>(Material))
	{
		auto *MID = UMaterialInstanceDynamic::Create(Material, GetTransientPackage());
		MID->CopyMaterialUniformParameters(MI);
		To->SetMaterial(0, MID);
		FVector Prev, Res;
		/*MID->GetScalarParameterValue(TEXT("R"), Res.X);
		MID->GetScalarParameterValue(TEXT("G"), Res.Y);
		MID->GetScalarParameterValue(TEXT("B"), Res.Z);*/
		UE_LOG(LogTemp,Warning,TEXT("%s"), *Res.ToString());
	}
}

void AMaterialRefactor::FindMaterialChild(UMaterialInterface* InMaterialInterface)
{
	if (!IsValid(InMaterialInterface))
	{
		return;
	}

	InMaterialInterface->MarkPackageDirty();

	TArray<UMeshComponent*> MeshComponents;
	
	for (TActorIterator<AActor> It(GetWorld());It;++It)
	{
		UE_LOG(LogTemp, Display, TEXT("Iter actor %s"), *It->GetName());

		auto AddComponentToArray = [&MeshComponents] (UMeshComponent *Component)
		{
			MeshComponents.Add(Component);
		};
		It->ForEachComponent<UStaticMeshComponent>(true, AddComponentToArray);
		It->ForEachComponent<USkeletalMeshComponent>(true, AddComponentToArray);
	}
	
	TArray<UMaterialInstance*> ChildrenMI; 
	TArray<FString> DebugStrings;
	for (auto *Component: MeshComponents)
	{
		bool flag=false;
		for (UMaterialInterface* Material: Component->GetMaterials())
		{
			if (!IsValid(Material))
			{
				continue;
			}

			DebugStrings.Empty();
			if (auto MaterialInstance = Cast<UMaterialInstance>(Material))
			{
				if (MaterialInstance->Parent == InMaterialInterface)
				{
					ChildrenMI.Add(MaterialInstance);
					flag=true;
					MaterialInstance->MarkPackageDirty();
				}
				
				DebugStrings.Add(Material->GetName() + "[" + Material->GetClass()->GetName() + "]" );
				
				// Material = MaterialInstance->Parent;
			}
			DebugStrings.Add(Material->GetName() );
			UE_LOG(LogTemp, Display, TEXT("%s"), *FString::Join(DebugStrings, TEXT(" -- ")));
			if (flag)
			{
				Component->SetMaterial(0,Material);
			}
		}
	}
	UE_LOG(LogTemp, Warning,TEXT("%d"), ChildrenMI.Num());
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


// void AMaterialRefactor2::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	TimeAcc+=DeltaTime;
// 	if (TimeAcc > 2. && IsValid(A) && IsValid(B))
// 	{
// 		TArray<UMeshComponent*> MeshComponents;
// 		A->GetComponents(MeshComponents);
//
// 		if (auto MID=Cast<UMaterialInstanceDynamic>(MeshComponents[0]->GetMaterial(0)))
// 		{
// 			MID->SetScalarParameterValue("G", 0.8);
// 		}
// 	}
// }

// void AMaterialRefactor2::CreateChildMID(AActor* lhs, AActor* rhs)
// {
// 	if (!lhs || !rhs) return;
// 	A = lhs, B = rhs;
// 	
// 	TArray<UMeshComponent*> MeshComponents, OutMeshComponents;
// 	lhs->GetComponents(MeshComponents);
// 	rhs->GetComponents(OutMeshComponents);
//
// 	if (MeshComponents.Num() == 0) return;
//
// 	auto *Material0=MeshComponents[0]->GetMaterial(0);
// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *Material0->GetClass()->GetName());
// 	auto MID = UMaterialInstanceDynamic::Create(Material0, GetTransientPackage());
// 	MID->CopyParameterOverrides(Cast<UMaterialInstance>(Material0));
// 	OutMeshComponents[0]->SetMaterial(0,MID);
// }
