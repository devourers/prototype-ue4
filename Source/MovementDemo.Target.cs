// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MovementDemoTarget : TargetRules
{
	public MovementDemoTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
        bOverrideBuildEnvironment = true;
        ExtraModuleNames.AddRange( new string[] { "MovementDemo" } );
	}
}
