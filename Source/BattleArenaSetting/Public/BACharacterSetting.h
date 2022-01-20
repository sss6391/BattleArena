// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BACharacterSetting.generated.h"

/**
 * 
 */
UCLASS(config=BattleArena)		// configŰ���� �߰�     config ������ DefaultBattleArena.ini ������ �о� ĳ���Ϳ��°��� ������
class BATTLEARENASETTING_API UBACharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
    UBACharacterSetting();

    UPROPERTY(config)
    TArray<FSoftObjectPath> CharacterAssets;
};
