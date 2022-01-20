// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BattleArenaTarget : TargetRules
{
	public BattleArenaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// "BattleArenaSetting" 모듈 추가		//BattleArena.uproject 파일에도 명시해준다
		//ExtraModuleNames.Add("BattleArena");  
		ExtraModuleNames.AddRange(new string[] { "BattleArena", "BattleArenaSetting" });            // "BattleArenaSetting" 모듈 추가 // AddRange(new string[]

	}
}
