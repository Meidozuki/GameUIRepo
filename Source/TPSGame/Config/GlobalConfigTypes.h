#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GlobalConfigTypes.generated.h"

USTRUCT(BlueprintType)
struct FConfigFloatTableRow: public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Value;
};

USTRUCT(BlueprintType)
struct FOptionalFloat
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	float Value;

	UPROPERTY(BlueprintReadOnly)
	bool Valid = false;
};
