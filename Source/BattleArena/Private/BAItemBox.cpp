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

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));		// 트리거 박스 크기 설정

	// 기존메쉬크기가 작아서 메쉬를 열어서 빌드 스케일을 1.5로 조정
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		SM_BOX(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	// 박스 열때 파티클 이펙트
	static ConstructorHelpers::FObjectFinder<UParticleSystem>
		P_CHEST_OPEN(TEXT("ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'"));
	if (P_CHEST_OPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHEST_OPEN.Object);
		Effect->bAutoActivate = false;
	}

	// 박스 위치 조절
	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	// 추가한 콜리전 프로파일 적용
	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));


	// 웨폰  기본 클래스 지정
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

	// BeginOverlap 델리케이드 바인딩
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
			// 캐릭터에 무기 스폰
			auto NewWeapon = GetWorld()->SpawnActor<ABAWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);		//WeaponItemClass 인자가 없으면 ABAWeapon의 cpp설정기본 값을 로드함
			BACharacter->SetWeapon(NewWeapon);
			
			// 이펙스 재생
			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);					// 에디터에서만 보임, Setvisivility는 게임과 에디터에서도 안보임,
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &ABAItemBox::OnEffectFinished);		// 효과가 사라지고 난뒤 아이템박스 액터 파괴
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


