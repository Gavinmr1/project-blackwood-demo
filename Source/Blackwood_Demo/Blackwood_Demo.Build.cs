// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Blackwood_Demo : ModuleRules
{
	public Blackwood_Demo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"Blackwood_Demo",
			"Blackwood_Demo/Variant_Horror",
			"Blackwood_Demo/Variant_Horror/UI",
			"Blackwood_Demo/Variant_Shooter",
			"Blackwood_Demo/Variant_Shooter/AI",
			"Blackwood_Demo/Variant_Shooter/UI",
			"Blackwood_Demo/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
