// Copyright @ 2023 Kamil Tokarski

using UnrealBuildTool;
using System.Collections.Generic;

public class AIGuysTarget : TargetRules
{
	public AIGuysTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "AIGuys" } );
	}
}
