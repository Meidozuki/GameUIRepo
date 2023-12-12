using UnrealBuildTool;

public class Utility: ModuleRules

{

	public Utility(ReadOnlyTargetRules Target) : base(Target)

	{

		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine"});

	}

}