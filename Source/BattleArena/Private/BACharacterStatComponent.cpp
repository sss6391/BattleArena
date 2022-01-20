// 게임인스턴스에 있는 데이터 테이블 참조
// 레벨에 맞는 체력 적용
// 캐릭터의 데미지 체력에 적용, 공격데미지량 전달, 죽으면 알리기
// 캐릭터내부에 붙어있는 컴포넌트


#include "BACharacterStatComponent.h"
#include "BAGameInstance.h"								// 테이블 데이터 구조체
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics 사용을 위해


// Sets default values for this component's properties
UBACharacterStatComponent::UBACharacterStatComponent()
{
	// 틱을 사용하지 않음, 생성시 기존에 있던 함수도 삭제
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent 함수를 사용하려면 이 변수를 true
	bWantsInitializeComponent = true;

	Level = 1;
}


// Called when the game starts
void UBACharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UBACharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// 기본 레벨 설정
	SetNewLevel(Level);
}

void UBACharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// 월드에서 사용하는 게임 인스턴스 불러오기
	auto BAGameInstance = Cast<UBAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	BACHECK(nullptr != BAGameInstance);
	CurrentStatData = BAGameInstance->GetBACharacterData(NewLevel);			// 게임인스턴스에 있는 데이터 테이블 참조, 맞는 레벨의 데이터 행을 모두 가져옴(구조체)

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);										
	}
	else
	{
		BALOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}

	// 레벨 확인
	BALOG(Warning, TEXT("New Level is (%d)"), NewLevel);
}

// 데미지를 받으면 현재 HP 에 적용
void UBACharacterStatComponent::SetDamage(float NewDamage)
{
	BACHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UBACharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	// KINDA_SMALL_NUMBER: 0에 근접한데 무시가능한 오차범위인지 확인, 0과 비교시 사용
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();				// HP가 0이면 브로드캐스트
	}
}

float UBACharacterStatComponent::GetAttack()
{
	BACHECK(nullptr != CurrentStatData, 0.0f);
	return CurrentStatData->Attack;
}

float UBACharacterStatComponent::GetHPRatio()
{
	BACHECK(nullptr != CurrentStatData, 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER)	?	0.0f	:	(CurrentHP / CurrentStatData->MaxHP);
}

int32 UBACharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}

