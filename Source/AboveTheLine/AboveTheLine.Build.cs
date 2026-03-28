// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class AboveTheLine : ModuleRules
{
	public AboveTheLine(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "StateTreeModule",
            "GameplayStateTreeModule",
            "AIModule",
            "UMG",
            "Slate",
            "SlateCore",
            "EngineCameras"
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] {
            "AboveTheLine",
            "AboveTheLine/Variant_Horror",
            "AboveTheLine/Variant_Horror/UI",
            "AboveTheLine/Variant_Shooter",
            "AboveTheLine/Variant_Shooter/AI",
            "AboveTheLine/Variant_Shooter/UI",
            "AboveTheLine/Variant_Shooter/Weapons"
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
