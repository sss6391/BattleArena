// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BattleArena : ModuleRules
{
	public BattleArena(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// "UMG" ��� �߰�, �׺����� ���� "NavigationSystem" ��� �߰�(BT�ý����̿����� AIModule�� ����), "GameplayTasks" BT�׽�ũ ����� ����
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

		// BattleArenaSetting ���� ������ ������ ������ �ֵ��� ��� ��� �߰�
		PublicDependencyModuleNames.AddRange(new string[] { "BattleArenaSetting" });
	}
}
