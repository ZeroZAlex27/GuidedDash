// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GuidedDash : ModuleRules
{
	public GuidedDash(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[] { 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput",
                "UMG",
                "Niagara",
                "NetCore",
                "AIModule"
            }
		);

		PrivateDependencyModuleNames.AddRange(
			new string[] {
                "GameplayAbilities", 
				"GameplayTags", 
				"GameplayTasks",
                "GameplayMessageRuntime"
            }
		);

        SetupIrisSupport(Target);
    }
}
