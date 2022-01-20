// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "GameFramework/PlayerState.h"
#include "BAPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangeDelegate);		// ���°� ���Ҷ����� ��ε�ĳ��Ʈ

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ABAPlayerState();

	int32 GetGameScore() const;
	int32 GetCharacterLevel() const;
	int32 GetGameHighScore() const;
	int32 GetCharacterIndex() const;
	float GetExpRatio() const;
	bool AddExp(int32 IncomeExp);
	void AddGameScore();			// ���ν��ھ� �ø���

	FOnPlayerStateChangeDelegate OnPlayerStateChanged;

	FString SaveSlotName;

	void InitPlayerData();			// ���� ������ ����
	void SavePlayerData();			// ������ ���̺�

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Exp;

	UPROPERTY(Transient)
	int32 GameHighScore;

	UPROPERTY(Transient)
	int32 CharacterIndex;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FBACharacterData* CurrentStatData;
};
