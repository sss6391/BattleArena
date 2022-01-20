// Fill out your copyright notice in the Description page of Project Settings.

//NextAttackCheck 라는 노티파이가 한 섹션이 끝나는 지점에서 가까워질수록 노티실행->다음섹션 명령을 내려도 OnMontageEnded 가 실행되서 콤보가 안될 확률이 높다.

#include "BAAnimInstance.h"

UBAAnimInstance::UBAAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// 콤보공격을 위한 몽타주 연결
	static ConstructorHelpers::FObjectFinder<UAnimMontage>
		ATTACK_MONTAGE(TEXT("/Game/Animations/SK_Mannequin_Skeleton_Montage.SK_Mannequin_Skeleton_Montage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UBAAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;

	if (!IsDead)
	{
		// 속도에 따른 애니메이션 재생 (블루프린터에서 설정해야함)
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			//IsFalling, IsSwimming, IsCrouching, IsMoveOnGround 함수는 캐릭터 무브먼트 컴포넌트에서만 완전히 사용가능, 다른 무브먼트는 false반환
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}


}

void UBAAnimInstance::PlayAttackMontage()
{
	// 몽타주가 실행중이라면 실행하지 않도록하지만 실제로 실행하는 캐릭터에서 델리케이트 함수를 통해 걸러내는게 좋다
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	// 캐릭터클래스에서 걸러내므로 상위의 소스 주석처리

	BACHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UBAAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	BACHECK(!IsDead);
	BACHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);		// 다음 몽타주로
}

void UBAAnimInstance::AnimNotify_AttackHitCheck()
{
	//BALOG_S(Warning);
	OnAttackHitCheck.Broadcast();
}

void UBAAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UBAAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	BACHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);

	return FName(*FString::Printf(TEXT("Attack%d"), Section));			// 어택 몽타주 Attack1, Attack2, Attack3, Attack4
}






