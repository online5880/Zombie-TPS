using UnrealBuildTool;
using System.Collections.Generic;

public class JuneServerTarget : TargetRules //Change this line according to the name of your project
{
	public JuneServerTarget(TargetInfo Target) : base(Target) //Change this line according to the name of your project
	{
		Type = TargetType.Server;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		ExtraModuleNames.Add("June"); //Change this line according to the name of your project
	}
}