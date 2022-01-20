// �����ν��Ͻ��� �ִ� ������ ���̺� ����
// ������ �´� ü�� ����
// ĳ������ ������ ü�¿� ����, ���ݵ������� ����, ������ �˸���
// ĳ���ͳ��ο� �پ��ִ� ������Ʈ


#include "BACharacterStatComponent.h"
#include "BAGameInstance.h"								// ���̺� ������ ����ü
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics ����� ����


// Sets default values for this component's properties
UBACharacterStatComponent::UBACharacterStatComponent()
{
	// ƽ�� ������� ����, ������ ������ �ִ� �Լ��� ����
	PrimaryComponentTick.bCanEverTick = false;

	// InitializeComponent �Լ��� ����Ϸ��� �� ������ true
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

	// �⺻ ���� ����
	SetNewLevel(Level);
}

void UBACharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	// ���忡�� ����ϴ� ���� �ν��Ͻ� �ҷ�����
	auto BAGameInstance = Cast<UBAGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	
	BACHECK(nullptr != BAGameInstance);
	CurrentStatData = BAGameInstance->GetBACharacterData(NewLevel);			// �����ν��Ͻ��� �ִ� ������ ���̺� ����, �´� ������ ������ ���� ��� ������(����ü)

	if (nullptr != CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);										
	}
	else
	{
		BALOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}

	// ���� Ȯ��
	BALOG(Warning, TEXT("New Level is (%d)"), NewLevel);
}

// �������� ������ ���� HP �� ����
void UBACharacterStatComponent::SetDamage(float NewDamage)
{
	BACHECK(nullptr != CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UBACharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();

	// KINDA_SMALL_NUMBER: 0�� �����ѵ� ���ð����� ������������ Ȯ��, 0�� �񱳽� ���
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();				// HP�� 0�̸� ��ε�ĳ��Ʈ
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

