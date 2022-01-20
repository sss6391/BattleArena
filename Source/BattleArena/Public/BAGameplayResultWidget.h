// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "BAGameplayWidget.h"
#include "BAGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBAGameplayResultWidget : public UBAGameplayWidget
{
	GENERATED_BODY()
	
public:
	void BindGameState(class ABAGameState* GameState);

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class ABAGameState> CurrentGameState;
};
