// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UniqueNPC : ModuleRules
{
	public UniqueNPC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"DeveloperSettings",
			"InputCore", 
			"EnhancedInput",
			"UMG",
			"GameplayTags",
			"AIModule",
			"NavigationSystem"
		});
	}
}
