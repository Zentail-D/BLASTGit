// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BLAST : ModuleRules
{
	public BLAST(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PrivateIncludePaths.AddRange(new string[] {
			"BLAST/",
			"BLAST/AI",
			"BLAST/AI/Engineer",
			"BLAST/AI/Engineer/BehaviorService",
			"BLAST/AI/Engineer/BehaviorTask",
			"BLAST/AI/Flying",
			"BLAST/AI/Flying/BehaviorService",
			"BLAST/AI/Flying/BehaviorTask",
			"BLAST/AI/Flying/Navigation",
			"BLAST/AI/Suicider",
			"BLAST/AI/Suicider/BehaviorTask",
			"BLAST/AI/Grunt",
			"BLAST/AI/Grunt/BehaviorService",
			"BLAST/AI/Grunt/BehaviorTask",
			"BLAST/AI/Mortars",
			"BLAST/AI/Mortars/BehaviorService",
			"BLAST/AI/Mortars/BehaviorTask",
			"BLAST/AI/ParentBehaviorService",
			"BLAST/AI/ParentBehaviorTask",
			"BLAST/Audio",
			"BLAST/Movement",
			"BLAST/WeaponSystem",
			"BLAST/WeaponSystem/ItemChildren",
			"BLAST/WeaponSystem/ItemChildren/Mods"
		});
		
		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"AIModule",
			"GameplayTasks",
			"NavigationSystem",
			"UMG",
			"Slate",
			"Niagara",
			"SlateCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ProceduralMeshComponent",
			"AIModule"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
