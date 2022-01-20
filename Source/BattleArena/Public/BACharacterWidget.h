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
	// ������Ʈ �����Ͽ� ������ Ȯ�� (���� ���Ͷ� ��������� �ٸ� ���Ͷ�� �ʼ� ���)
	TWeakObjectPtr<class UBACharacterStatComponent> CurrentCharacterStat;
	
	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
