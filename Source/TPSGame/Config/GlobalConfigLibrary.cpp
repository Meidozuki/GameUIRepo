// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalConfigLibrary.h"


FOptionalFloat UGlobalConfigLibrary::ReadFloatConfig(FName Name)
{
	constexpr auto TableName = TEXT("/Script/Engine.DataTable'/Game/GameName/Config/DT_GlobalFloatConfig.DT_GlobalFloatConfig'");
	if (auto *DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TableName)))
	{
		if (auto *Res = DataTable->FindRow<FConfigFloatTableRow>(Name, FString()))
		{
			return {Res->Value, true};
		}
	}
	return {{}, false};
}


