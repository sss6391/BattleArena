// Fill out your copyright notice in the Description page of Project Settings.
// 
// WarriorAnimBlueprint �� �θ� Ŭ����


// NextAttackCheck ��Ƽ������ ��ġ�� �� ������ ������ ������ ����������� ��Ƽ���̽���->�������� ����� ������ OnMontageEnded �� ����Ǽ� �޺��� �ȵ� Ȯ���� ����. 

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BAAnimInstance.generated.h"

// �޺��� ���� ��������Ʈ ����
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);

/**
 * 
 */
UCLASS()
class BATTLEARENA_API UBAAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBAAnimInstance();
	// �����Ͻ� ȣ���, (CurrentPawnSpeed �ٸ� ������� ������ ������Ʈ�ϴ� ��ĵ� ����), 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	// ����
	void SetDeadAnim() { IsDead = true; }

	// ��������Ʈ ���� (�ٸ������� �����ؼ� ����Ҽ��ְ�(���)) 
	FOnNextAttackCheckDelegate OnNextAttackCheck;				
	FOnAttackHitCheckDelegate OnAttackHitCheck;						

private:
	// �������Ʈ ��Ƽ���̸� �߰���(AnimNotify_0000 �߰��� ��Ƽ���� �̸��� ����)
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();			// �޺�Ȯ��

	// ���� �޺����
	FName GetAttackMontageSectionName(int32 Section);


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Pawn, Meta=(AllowPrivateAccess=True))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = True))
	bool IsInAir;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = True))
	UAnimMontage* AttackMontage;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = True))
	bool IsDead;

};
