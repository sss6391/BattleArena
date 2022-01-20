// Fill out your copyright notice in the Description page of Project Settings.

//NextAttackCheck ��� ��Ƽ���̰� �� ������ ������ �������� ����������� ��Ƽ����->�������� ����� ������ OnMontageEnded �� ����Ǽ� �޺��� �ȵ� Ȯ���� ����.

#include "BAAnimInstance.h"

UBAAnimInstance::UBAAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	IsDead = false;

	// �޺������� ���� ��Ÿ�� ����
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
		// �ӵ��� ���� �ִϸ��̼� ��� (��������Ϳ��� �����ؾ���)
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character)
		{
			//IsFalling, IsSwimming, IsCrouching, IsMoveOnGround �Լ��� ĳ���� �����Ʈ ������Ʈ������ ������ ��밡��, �ٸ� �����Ʈ�� false��ȯ
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}


}

void UBAAnimInstance::PlayAttackMontage()
{
	// ��Ÿ�ְ� �������̶�� �������� �ʵ��������� ������ �����ϴ� ĳ���Ϳ��� ��������Ʈ �Լ��� ���� �ɷ����°� ����
	//if (!Montage_IsPlaying(AttackMontage))
	//{
	//	Montage_Play(AttackMontage, 1.0f);
	//}
	// ĳ����Ŭ�������� �ɷ����Ƿ� ������ �ҽ� �ּ�ó��

	BACHECK(!IsDead);
	Montage_Play(AttackMontage, 1.0f);
}

void UBAAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	BACHECK(!IsDead);
	BACHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);		// ���� ��Ÿ�ַ�
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

	return FName(*FString::Printf(TEXT("Attack%d"), Section));			// ���� ��Ÿ�� Attack1, Attack2, Attack3, Attack4
}






