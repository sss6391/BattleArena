// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include <Engine/Classes/GameFramework/SpringArmComponent.h>
#include <Engine/Classes/GameFramework/CharacterMovementComponent.h>	// GetCharacterMovement
#include <Engine/Classes/Camera/CameraComponent.h>
#include <Engine/Classes/Components/CapsuleComponent.h>
#include <Engine/Classes/Kismet/KismetMathLibrary.h>					// 방향 테스트용
#include "BAAnimInstance.h"												// 커스텀으로 만든 애님인스턴스, 애님 블루브린터의 부모클래스
#include "BAWeapon.h"													// 커스텀으로 만든 무기 액터
#include "BACharacterStatComponent.h"									// 테이블 데이터와 연결할 엑터 컴포넌트
#include "GameFramework/Character.h"
#include "BACharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);			// NPC의 공격과정이 끝나는 지점을 알려줌 (BTTask_Attack에서 사용하도록함)


UCLASS()
class BATTLEARENA_API ABACharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABACharacter();

	// 상태 (스테이트 머신)
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

	// 부드러운 카메라 전환을 위해
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

	// 델리게이트 바인딩시 이 함수에서 선언
	virtual void PostInitializeComponents() override;

	// 데미지 처리
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// NPC인지 아닌지에 따른 컨트롤 전환
	virtual void PossessedBy(AController* NewController) override;

	// 무기 관련
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

	// 테이블데이터를 갖고 있는 상태컴포넌트
	UPROPERTY(VisibleAnywhere, Category = Stat)
	class UBACharacterStatComponent* CharacterStat;

	// BattleArena.Build.cs에서 "UMG" 모듈 추가
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UWidgetComponent* HPBarWidget;


	// 콤보변수
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
	
	// 디버그 공격범위 그리기용 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	float AttackRange;

	// 디버그 공격범위 그리기용
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPriveteAccess = ture))
	float AttackRadius;
	
	// 공격
	void Attack();
	FOnAttackEndDelegate OnAttackEnd;		// OnAttackMontageEnded 에서 브로드캐스트, BTTask_Attack에서 사용 바인딩

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

	// 비동기 애셋로딩, 게임인스턴스와 연결
	void OnAssetLoadCompleted();
	FSoftObjectPath CharacterAssetToLoad = FSoftObjectPath(nullptr);
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	////

	// 상태 인덱스
	int32 AssetIndex = 0;

	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, Category = State, Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadonly, Category = State, Meta = (AllowPrivateAccess = true))
	bool bIsPlayer;

	UPROPERTY()
	class ABAAIController* BAAIController;

	UPROPERTY()
	class ABAPlayerController* BAPlayerController;


	// 죽을때 사라지는 타이머
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle = {};
};
