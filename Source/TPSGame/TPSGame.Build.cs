// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TPSGame : ModuleRules
{
	public TPSGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		CppStandard = CppStandardVersion.Cpp20;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Utility", "NavigationSystem", "AIModule", "Niagara"
		});
		
		PrivateIncludePaths.AddRange(new string[]
		{
			ModuleDirectory
		});
	}
}
