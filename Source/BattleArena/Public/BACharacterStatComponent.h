// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Components/ActorComponent.h"
#include "BACharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);			// 케릭터의 HP가 0이하면 브로드캐스트
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);			// 캐릭터의 HP가 변할때마다 브로드캐스트



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLEARENA_API UBACharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBACharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// 엑터의 PostInitializeComponents() 호출되기 바로 전에 호출됨
	virtual void InitializeComponent() override;

public:	
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	void SetHP(float NewHP);
	float GetAttack();
	float GetHPRatio();
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private:
	struct FBACharacterData* CurrentStatData = nullptr;

	UPROPERTY(EditInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;
		
	// Transient 옵션을 주어서 직렬화기능을 삭제
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
