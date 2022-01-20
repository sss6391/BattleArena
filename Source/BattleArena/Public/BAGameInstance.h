// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Engine/DaTaTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"				// FStreamableManager �񵿱� �ּ·ε�
#include "BAGameInstance.generated.h"

// ���������̺� ����ü ����
USTRUCT(BlueprintType)
struct FBACharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// ù���� �̸��� Ű������ ���̱� ������ ������ �ʿ䰡 ����
	FBACharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};


/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UBAGameInstance();

	virtual void Init() override;
	FBACharacterData* GetBACharacterData(int32 Level);


	FStreamableManager StreamableManager;				// �񵿱� �ּ·ε�

private:

	// UDataTable�� �̿��Ͽ� ���̺����͸� �����Ѵ�.
	UPROPERTY()
	class UDataTable* BACharacterData;
};
