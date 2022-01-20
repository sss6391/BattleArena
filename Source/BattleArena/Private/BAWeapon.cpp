// Fill out your copyright notice in the Description page of Project Settings.


#include "BAWeapon.h"

// Sets default values
ABAWeapon::ABAWeapon()
{
	PrimaryActorTick.bCanEverTick = false;			// ƽ ����

	// ���� Į �޽� ����
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_WEAPON(TEXT("/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight"));

	if (SK_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SK_WEAPON.Object);			// �ҷ��� �޽��� ������Ʈ�� ����
	}

	// �� ������Ʈ������ ����� �ݸ������� ������� ����
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));

	// ���⿡ ���� ���� �߰� ������ ��ȭ
	AttackRange = 150;					// ���� �⺻ ���� ����
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