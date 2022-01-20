// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BACharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=BattleArena)		// config키워드 추가     config 폴더속 DefaultBattleArena.ini 파일을 읽어 캐릭터에셋값을 설정함
class BATTLEARENASETTING_API UBACharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
    UBACharacterSetting();

    UPROPERTY(config)
    TArray<FSoftObjectPath> CharacterAssets;
};
