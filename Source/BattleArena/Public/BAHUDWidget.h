// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "Blueprint/UserWidget.h"
#include "BAHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBAHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void BindCharacterStat(class UBACharacterStatComponent* CharacterStat);
	void BindPlayerState(class ABAPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<class UBACharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<class ABAPlayerState> CurrentPlayerState;

	UPROPERTY()
	class UProgressBar* HPBar;

	UPROPERTY()
	class UProgressBar* ExpBar;

	UPROPERTY()
	class UTextBlock* PlayerName;

	UPROPERTY()
	class UTextBlock* PlayerLevel;

	UPROPERTY()
	class UTextBlock* CurrentScore;

	UPROPERTY()
	class UTextBlock* HighScore;
};
