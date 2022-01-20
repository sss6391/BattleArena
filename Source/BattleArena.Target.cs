// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BattleArenaTarget : TargetRules
{
	public BattleArenaTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// "BattleArenaSetting" ��� �߰�		//BattleArena.uproject ���Ͽ��� ������ش�
		//ExtraModuleNames.Add("BattleArena");  
		ExtraModuleNames.AddRange(new string[] { "BattleArena", "BattleArenaSetting" });            // "BattleArenaSetting" ��� �߰� // AddRange(new string[]

	}
}
