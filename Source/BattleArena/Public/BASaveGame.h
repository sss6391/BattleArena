// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "GameFramework/SaveGame.h"
#include "BASaveGame.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBASaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UBASaveGame();

	UPROPERTY()
	int32 CharacterIndex;

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 HighScore;
	
};
