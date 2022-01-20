// 게임모드에서 관리, 게임안에서의 상태 저장 처리, 단일엑터

#include "BAPlayerState.h"
#include "BAGameInstance.h"
#include "BASaveGame.h"									// 게임 저장
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics 사용을 위해


ABAPlayerState::ABAPlayerState()
{
	CharacterLevel = 1;
	GameScore = 0;
	Exp = 0;
	GameHighScore = 0;
	SaveSlotName = TEXT("Player1");
}

// 스테이트 초기화
void ABAPlayerState::InitPlayerData()
{
	// 게임 저장슬롯 초기 세팅
	auto ABSaveGame = Cast<UBASaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));			// 저장슬롯에 있는 데이터 불러오기
	if (nullptr == ABSaveGame)
	{
		ABSaveGame = GetMutableDefault<UBASaveGame>();
	}

	SetPlayerName(ABSaveGame->PlayerName);				// SetPlayerName() 스테이트 자체 내부 변수에 저장
	SetCharacterLevel(ABSaveGame->Level);
	GameScore = 0;
	GameHighScore = ABSaveGame->HighScore;
	Exp = ABSaveGame->Exp;
	CharacterIndex = ABSaveGame->CharacterIndex;

	SavePlayerData();									// 처음이라면 세이브파일이 ??
}

// 경험치를 얻거나 스코어가 올라갈때마다 저장되도록
void ABAPlayerState::SavePlayerData()
{
	UBASaveGame* NewPlayerData = NewObject<UBASaveGame>();				
	NewPlayerData->PlayerName = GetPlayerName();
	NewPlayerData->Level = CharacterLevel;
	NewPlayerData->Exp = Exp;
	NewPlayerData->HighScore = GameHighScore;
	NewPlayerData->CharacterIndex = CharacterIndex;

	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, SaveSlotName, 0))
	{
		//BALOG(Error, TEXT("SaveGame Error!"));			// 에러는 뜨는데 세이브는 되고 있음
	}
}

void ABAPlayerState::AddGameScore()
{
	GameScore++;
	if (GameScore >= GameHighScore)
	{
		GameHighScore = GameScore;
	}
	OnPlayerStateChanged.Broadcast();

	SavePlayerData();
}

int32 ABAPlayerState::GetGameHighScore() const
{
	return GameHighScore;
}

int32 ABAPlayerState::GetCharacterIndex() const
{
	return CharacterIndex;
}

int32 ABAPlayerState::GetGameScore() const
{
	return GameScore;
}

int32 ABAPlayerState::GetCharacterLevel() const
{
	return CharacterLevel;
}

float ABAPlayerState::GetExpRatio() const
{
	if (CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
		return 0.0f;

	float Result = (float)Exp / (float)CurrentStatData->NextExp;

	BALOG(Warning, TEXT("Ration : %f, Current : %d, Next : %d"), Result, Exp, CurrentStatData->NextExp);

	return Result;
}

// 플레이어 컨트롤러에서 호출함
bool ABAPlayerState::AddExp(int32 IncomeExp)
{
	if (CurrentStatData->NextExp == -1)
	{
		return false;
	}

	bool DidLevelUp = false;
	Exp = Exp + IncomeExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();

	SavePlayerData();
	return DidLevelUp;
}


void ABAPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	auto BAGameInstance = Cast<UBAGameInstance>(GetGameInstance());
	BACHECK(nullptr != BAGameInstance);

	CurrentStatData = BAGameInstance->GetBACharacterData(NewCharacterLevel);		// 레벨에 맞는 행 데이터 가져오기(구조체), 스테이트 컴포넌트는 자신만의 데이터를 불러오도록함
	BACHECK(nullptr != CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
