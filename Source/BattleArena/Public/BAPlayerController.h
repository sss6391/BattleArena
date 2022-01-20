// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "GameFramework/PlayerController.h"
#include "BAPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABAPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;			// 4.22부터 Possess에서 OnPossess로 변경

	class UBAHUDWidget* GetHUDWidget() const;				// BAHUDWidget 생성용

	void NPCKill(class ABACharacter* KilledNPC) const;		// NPC를 죽였을때, 경험치 적용
	void AddGameScore() const;
	void ChangeInputMode(bool bGameMode = true);
	void ShowResultUI();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;					// 게임 일시정지 키 바인딩

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UBAHUDWidget> HUDWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UBAGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UBAGameplayResultWidget> ResultWidgetClass;

private:
	void OnGamePause();

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly   UIInputMode;

	UPROPERTY()
	class UBAHUDWidget* HUDWidget;

	UPROPERTY()
	class ABAPlayerState* BAPlayerState;

	UPROPERTY()
	class UBAGameplayWidget* MenuWidget;

	UPROPERTY()
	class UBAGameplayResultWidget* ResultWidget;
};
