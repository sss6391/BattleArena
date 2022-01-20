// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleArena.h"
#include "BAGameMode.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABAGameMode();

	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void AddScore(class ABAPlayerController* ScoredPlayer);
	int32 GetTotalScore() const;

private:
	UPROPERTY()
	class ABAGameState* BAGameState;

	UPROPERTY()
	int32 ScoreToClear;
};
