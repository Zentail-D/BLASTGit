// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BlastTest : ModuleRules
{
	public BlastTest(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.AddRange(new string[]
		{
			"BlastTest/",
			"BlastTest/ZoneTool/",
			"BlastTest/AI",
			"BlastTest/AI/Engineer",
			"BlastTest/AI/Engineer/BehaviorService",
			"BlastTest/AI/Engineer/BehaviorTask",
			"BlastTest/AI/Flying",
			"BlastTest/AI/Flying/BehaviorService",
			"BlastTest/AI/Flying/BehaviorTask",
			"BlastTest/AI/Flying/Navigation",
			"BlastTest/AI/Suicider",
			"BlastTest/AI/Suicider/BehaviorTask",
			"BlastTest/AI/Grunt",
			"BlastTest/AI/Grunt/BehaviorService",
			"BlastTest/AI/Grunt/BehaviorTask",
			"BlastTest/AI/Mortars",
			"BlastTest/AI/Mortars/BehaviorService",
			"BlastTest/AI/Mortars/BehaviorTask",
			"BlastTest/AI/ParentBehaviorService",
			"BlastTest/AI/ParentBehaviorTask",
			"BlastTest/WeaponTest",
			"BlastTest/WeaponTest/WeaponSystem",
			"BlastTest/WeaponTest/WeaponSystem/ItemChildren",
			"BlastTest/WeaponTest/WeaponSystem/ItemChildren/Mods",
			"BlastTest/Movement"
		});
		
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
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
			"HeadMountedDisplay",
			"SlateCore" });
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ProceduralMeshComponent",
			"AIModule"
		});
	}
	
	
}
