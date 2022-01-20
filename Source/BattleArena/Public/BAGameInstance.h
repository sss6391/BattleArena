// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Engine/DaTaTable.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"				// FStreamableManager 비동기 애셋로딩
#include "BAGameInstance.generated.h"

// 데이터테이블 구조체 선언
USTRUCT(BlueprintType)
struct FBACharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	// 첫줄인 이름은 키값으로 쓰이기 때문에 선언할 필요가 없다
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


	FStreamableManager StreamableManager;				// 비동기 애셋로딩

private:

	// UDataTable을 이용하여 테이블데이터를 관리한다.
	UPROPERTY()
	class UDataTable* BACharacterData;
};
