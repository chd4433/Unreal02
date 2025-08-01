// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class U2505_98 : ModuleRules
{
	public U2505_98(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add(ModuleDirectory);

        PublicDependencyModuleNames.Add("Core");
        PublicDependencyModuleNames.Add("CoreUObject");
        PublicDependencyModuleNames.Add("Engine");
        PublicDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("AIModule");
        PublicDependencyModuleNames.Add("NavigationSystem");
        PublicDependencyModuleNames.Add("MotionWarping");

    }
}
