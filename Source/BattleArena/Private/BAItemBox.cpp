// Fill out your copyright notice in the Description page of Project Settings.


#include "BAItemBox.h"
#include "BAWeapon.h"
#include "BACharacter.h"

// Sets default values
ABAItemBox::ABAItemBox()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));
	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);
	Effect->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));		// Ʈ���� �ڽ� ũ�� ����

	// �����޽�ũ�Ⱑ �۾Ƽ� �޽��� ��� ���� �������� 1.5�� ����
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BOX(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// �ڽ� ���� ��ƼŬ ����Ʈ
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		P_CHEST_OPEN(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHEST_OPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHEST_OPEN.Object);
		Effect->bAutoActivate = false;
	}

	// �ڽ� ��ġ ����
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// �߰��� �ݸ��� �������� ����
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));


	// ����  �⺻ Ŭ���� ����
	WeaponItemClass = ABAWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ABAItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABAItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// BeginOverlap �������̵� ���ε�
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABAItemBox::OnCharacterOverlap);
}

void ABAItemBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BALOG_S(Warning);

	auto BACharacter = Cast<ABACharacter>(OtherActor);
	BACHECK(nullptr != BACharacter);

	if (nullptr != BACharacter && nullptr != WeaponItemClass)
	{
		if (BACharacter->CanSetWeapon())
		{
			// ĳ���Ϳ� ���� ����
			auto NewWeapon = GetWorld()->SpawnActor<ABAWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);		//WeaponItemClass ���ڰ� ������ ABAWeapon�� cpp�����⺻ ���� �ε���
			BACharacter->SetWeapon(NewWeapon);
			
			// ���彺 ���
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);					// �����Ϳ����� ����, Setvisivility�� ���Ӱ� �����Ϳ����� �Ⱥ���,
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &ABAItemBox::OnEffectFinished);		// ȿ���� ������� ���� �����۹ڽ� ���� �ı�
		}
		else
		{
			BALOG(Warning, TEXT("%s Can't equip Weapon currently."), *BACharacter->GetName());
		}
	}
}

void ABAItemBox::OnEffectFinished(UParticleSystemComponent* PSystem)
{
	Destroy();
}


