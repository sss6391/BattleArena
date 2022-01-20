// Fill out your copyright notice in the Description page of Project Settings.


#include "BAWeapon.h"

// Sets default values
ABAWeapon::ABAWeapon()
{
	PrimaryActorTick.bCanEverTick = false;			// 틱 끄기

	// 검은 칼 메쉬 설정
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));

	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);			// 불러온 메쉬를 컴포넌트에 설정
	}

	// 이 프로젝트에서는 무기는 콜리전으로 사용하지 않음
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// 무기에 따른 랜덤 추가 데미지 변화
	AttackRange = 150;					// 무기 기본 공격 범위
	AttackDamageMin = -2.5f;			
	AttackDamageMax = 10.0f;			
	AttackModifierMin = 0.85f;
	AttackModifierMax = 1.25f;
}

float ABAWeapon::GetAttackRange() const
{
	return AttackRange;
}

float ABAWeapon::GetAttackDamage() const
{
	return AttackDamage;
}

float ABAWeapon::GetAttackModifier() const
{
	return AttackModifier;
}

// Called when the game starts or when spawned
void ABAWeapon::BeginPlay()
{
	Super::BeginPlay();
	
	AttackDamage = FMath::RandRange(AttackDamageMin, AttackDamageMax);
	AttackModifier = FMath::RandRange(AttackModifierMin, AttackModifierMax);
	
	BALOG(Warning, TEXT("Weapon Damage : %f, Modifier : %f"), AttackDamage, AttackModifier);
}