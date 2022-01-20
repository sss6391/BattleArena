// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Blueprint/UserWidget.h"
#include "BACharacterWidget.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBACharacterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(class UBACharacterStatComponent* NewCharacterStat);

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();


private:
	// 약포인트 선언하여 안전성 확보 (같은 엑터라서 상관없지만 다른 엑터라면 필수 사용)
	TWeakObjectPtr<class UBACharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
