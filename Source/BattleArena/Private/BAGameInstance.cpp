// ��&��忡�� �����ν��Ͻ� �׸��� �������ش�
// �ε� ����: gameinstance::Init -> AActor::Postinitalizecomponets -> AGameMode:PosetLogin -> AGameMode::StartPlay, AActor::BeginePlay
// �������� �������� ���� ��� �����ϸ� ������ ���̺��� ������ �ִ´�

#include "BAGameInstance.h"

UBAGameInstance::UBAGameInstance()
{
	// ������ ���̺� �ε�
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");			// CSV������ ����Ʈ ���ѳ���
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BACHARACTER(*CharacterDataPath);
	BACHECK(DT_BACHARACTER.Succeeded());
	BACharacterData = DT_BACHARACTER.Object;
	BACHECK(BACharacterData->GetRowMap().Num() > 0);
}

void UBAGameInstance::Init()
{
	Super::Init();

	// ������ ��� Ȯ��
	//BALOG(Warning, TEXT("DropExp of Level 20 BACharacter : %d") , GetBACharacterData(20)->DropExp);
}

// �Էµ� ������ �´� �� ������ �ҷ�����
FBACharacterData* UBAGameInstance::GetBACharacterData(int32 Level)
{
	return BACharacterData->FindRow<FBACharacterData>(*FString::FromInt(Level),TEXT(""));
}
