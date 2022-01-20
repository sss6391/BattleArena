// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Components/ActorComponent.h"
#include "BACharacterStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);			// �ɸ����� HP�� 0���ϸ� ��ε�ĳ��Ʈ
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);			// ĳ������ HP�� ���Ҷ����� ��ε�ĳ��Ʈ



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

	// ������ PostInitializeComponents() ȣ��Ǳ� �ٷ� ���� ȣ���
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
		
	// Transient �ɼ��� �־ ����ȭ����� ����
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
