// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class BattleArenaEditorTarget : TargetRules
{
	public BattleArenaEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// "BattleArenaSetting" ��� �߰�		//BattleArena.uproject ���Ͽ��� ������ش�
		//ExtraModuleNames.Add("BattleArena", "BattleArenaSetting");
		ExtraModuleNames.AddRange(new string[] { "BattleArena", "BattleArenaSetting" });            // "BattleArenaSetting" ��� �߰�
	}
}
