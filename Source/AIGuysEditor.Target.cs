// Copyright @ 2023 Kamil Tokarski

using UnrealBuildTool;
using System.Collections.Generic;

public class AIGuysEditorTarget : TargetRules
{
	public AIGuysEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "AIGuys" } );
	}
}
