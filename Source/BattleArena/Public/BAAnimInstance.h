// Fill out your copyright notice in the Description page of Project Settings.
// 
// WarriorAnimBlueprint 의 부모 클래스


// NextAttackCheck 노티파이의 위치가 한 섹션이 끝나는 지점에 가까워질수록 노티파이실행->다음섹션 명령을 내려도 OnMontageEnded 가 실행되서 콤보가 안될 확률이 높다. 

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BAAnimInstance.generated.h"

// 콤보를 위한 델리게이트 선언
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
	// 움직일시 호출됨, (CurrentPawnSpeed 다른 방법으론 폰에서 업데이트하는 방식도 가능), 
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);

	// 죽음
	void SetDeadAnim() { IsDead = true; }

	// 델리케이트 변수 (다른곳에서 접근해서 사용할수있게(등록)) 
	FOnNextAttackCheckDelegate OnNextAttackCheck;				
	FOnAttackHitCheckDelegate OnAttackHitCheck;						

private:
	// 블루프린트 노티파이를 추가함(AnimNotify_0000 추가한 노티파이 이름이 붙음)
	UFUNCTION()
	void AnimNotify_AttackHitCheck();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();			// 콤보확인

	// 다음 콤보재생
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
