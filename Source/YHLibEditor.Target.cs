// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class YHLibEditorTarget : TargetRules
{
	public YHLibEditorTarget(TargetInfo Target):base(Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("YHLib");
    }

	////
	//// TargetRules interface.
	////

	//public override void SetupBinaries(
	//	TargetInfo Target,
	//	ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
	//	ref List<string> OutExtraModuleNames
	//	)
	//{
	//	OutExtraModuleNames.AddRange( new string[] { "YHLib" } );
	//}
}
