// 맵&모드에서 게임인스턴스 항목을 변경해준다
// 로드 순서: gameinstance::Init -> AActor::Postinitalizecomponets -> AGameMode:PosetLogin -> AGameMode::StartPlay, AActor::BeginePlay
// 레벨에서 없어지지 없고 계속 존재하며 데이터 테이블을 가지고 있는다

#include "BAGameInstance.h"

UBAGameInstance::UBAGameInstance()
{
	// 데이터 테이블 로드
	FString CharacterDataPath = TEXT("/Game/Book/GameData/ABCharacterData.ABCharacterData");			// CSV파일을 임포트 시켜놓음
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_BACHARACTER(*CharacterDataPath);
	BACHECK(DT_BACHARACTER.Succeeded());
	BACharacterData = DT_BACHARACTER.Object;
	BACHECK(BACharacterData->GetRowMap().Num() > 0);
}

void UBAGameInstance::Init()
{
	Super::Init();

	// 데이터 출력 확인
	//BALOG(Warning, TEXT("DropExp of Level 20 BACharacter : %d") , GetBACharacterData(20)->DropExp);
}

// 입력된 레벨에 맞는 행 데이터 불러오기
FBACharacterData* UBAGameInstance::GetBACharacterData(int32 Level)
{
	return BACharacterData->FindRow<FBACharacterData>(*FString::FromInt(Level),TEXT(""));
}
