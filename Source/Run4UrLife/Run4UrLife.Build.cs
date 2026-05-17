// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Run4UrLife : ModuleRules
{
	public Run4UrLife(ReadOnlyTargetRules Target) : base(Target)
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
			"Run4UrLife",
			"Run4UrLife/Variant_Platforming",
			"Run4UrLife/Variant_Platforming/Animation",
			"Run4UrLife/Variant_Combat",
			"Run4UrLife/Variant_Combat/AI",
			"Run4UrLife/Variant_Combat/Animation",
			"Run4UrLife/Variant_Combat/Gameplay",
			"Run4UrLife/Variant_Combat/Interfaces",
			"Run4UrLife/Variant_Combat/UI",
			"Run4UrLife/Variant_SideScrolling",
			"Run4UrLife/Variant_SideScrolling/AI",
			"Run4UrLife/Variant_SideScrolling/Gameplay",
			"Run4UrLife/Variant_SideScrolling/Interfaces",
			"Run4UrLife/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
