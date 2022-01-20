// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/GameFramework/CharacterMovementComponent.h>	// GetCharacterMovement
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Kismet/KismetMathLibrary.h>					// ���� �׽�Ʈ��
#include "BAAnimInstance.h"												// Ŀ�������� ���� �ִ��ν��Ͻ�, �ִ� ���기���� �θ�Ŭ����
#include "BAWeapon.h"													// Ŀ�������� ���� ���� ����
#include "BACharacterStatComponent.h"									// ���̺� �����Ϳ� ������ ���� ������Ʈ
#include "GameFramework/Character.h"
#include "BACharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);			// NPC�� ���ݰ����� ������ ������ �˷��� (BTTask_Attack���� ����ϵ�����)


UCLASS()
class BATTLEARENA_API ABACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABACharacter();

	// ���� (������Ʈ �ӽ�)
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void SetControlMode(int32 ControlMode);
	
	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC,
	};
	
	void SetControlMode(EControlMode NewControlMode);
	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;

	// �ε巯�� ī�޶� ��ȯ�� ����
	float fArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float fArmLengthSpeed = 0.0f;
	float fArmRotationSpeed = 0.0f;

	//test
	FRotator ControlRot = FRotator::ZeroRotator;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ��������Ʈ ���ε��� �� �Լ����� ����
	virtual void PostInitializeComponents() override;

	// ������ ó��
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// NPC���� �ƴ����� ���� ��Ʈ�� ��ȯ
	virtual void PossessedBy(AController* NewController) override;

	// ���� ����
	bool CanSetWeapon();
	void SetWeapon(class ABAWeapon* NewWeapon);
	float GetFinalAttackRange() const;
	float GetFinalAttackDamage() const;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	class ABAWeapon* CurrentWeapon;

	//UPROPERTY(VisibleAnywhere, Category = Weapon)
	//USkeletalMeshComponent* Weapon;
	/////////////
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;

	// ���̺����͸� ���� �ִ� ����������Ʈ
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UBACharacterStatComponent* CharacterStat;

	// BattleArena.Build.cs���� "UMG" ��� �߰�
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UWidgetComponent* HPBarWidget;


	// �޺�����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	bool CanNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	int32 MaxCombo;
	/////////////
	
	// ����� ���ݹ��� �׸���� 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	float AttackRange;

	// ����� ���ݹ��� �׸����
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	float AttackRadius;
	
	// ����
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;		// OnAttackMontageEnded ���� ��ε�ĳ��Ʈ, BTTask_Attack���� ��� ���ε�

	int32 GetExp() const;

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);

	void ViewChange();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void AttackCheck();

	UPROPERTY()
	class UBAAnimInstance* BAAnim;

	// �񵿱� �ּ·ε�, �����ν��Ͻ��� ����
	void OnAssetLoadCompleted();
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	////

	// ���� �ε���
	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class ABAAIController* BAAIController;

	UPROPERTY()
	class ABAPlayerController* BAPlayerController;


	// ������ ������� Ÿ�̸�
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};
};
