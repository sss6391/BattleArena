// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class BattleArena : ModuleRules
{
	public BattleArena(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// "UMG" 모듈 추가, 네비사용을 위한 "NavigationSystem" 모듈 추가(BT시스템이용으로 AIModule로 변경), "GameplayTasks" BT테스크 사용을 위해
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "NavigationSystem", "AIModule", "GameplayTasks" });

		// BattleArenaSetting 에서 지정한 에셋을 읽을수 있도록 모듈 목록 추가
		PublicDependencyModuleNames.AddRange(new string[] { "BattleArenaSetting" });
	}
}
