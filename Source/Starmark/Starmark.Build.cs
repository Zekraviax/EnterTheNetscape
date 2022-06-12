// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Starmark : ModuleRules
{
	public Starmark(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"UMG",
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule",
            "OnlineSubsystem",
            "OnlineSubsystemSteam" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        //DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        // Uncomment if you are using Slate UI
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        bLegacyPublicIncludePaths = false;
    }
}
