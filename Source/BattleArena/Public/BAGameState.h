// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "GameFramework/GameStateBase.h"
#include "BAGameState.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABAGameState();

public:
	int32 GetTotalGameScore() const;
	void AddGameScore();
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	UPROPERTY(Transient)
	int32 TotalGameScore;

	UPROPERTY(Transient)
	bool bGameCleared;
};
