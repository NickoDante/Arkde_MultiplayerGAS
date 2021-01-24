// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Arkde_MultiplayerGAS : ModuleRules
{
	public Arkde_MultiplayerGAS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
