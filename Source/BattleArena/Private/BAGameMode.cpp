// 게임의 기본 클래스 지정, 초기화, 클리어시 동작

#include "BAGameMode.h"
//#include "BAPawn.h"				// 폰 기본으로 (테스트용)
#include "BACharacter.h"			// 폰
#include "BAPlayerController.h"		// 컨트롤러 기본
#include "BAPlayerState.h"			// 상태 기본
#include "BAGameState.h"			// 게임 전역 상태

ABAGameMode::ABAGameMode()
{
	//DefaultPawnClass = ABAPawn::StaticClass();
	DefaultPawnClass = ABACharacter::StaticClass();					// 폰 클래스 지정
	PlayerControllerClass = ABAPlayerController::StaticClass();		// 컨트롤러 클래스 지정
	PlayerStateClass = ABAPlayerState::StaticClass();				// 상태 클래스 지정
	GameStateClass = ABAGameState::StaticClass();					// 게임 스테이트 클래스 지정

	ScoreToClear = 10;				// 클리어 점수 설정
}

void ABAGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BAGameState = Cast<ABAGameState>(GameState);		// 지정한 게임스테이트를 가져옴
}

// 플레이어 컨트롤러 생성-> 플레이어 폰 생성-> 빙의 -> 게임시작
// 컴포넌트 생성-> GameMode::PostLogin Begin ->Pawn 생성->빙의->GameMode::PostLogin End 순서
void ABAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// 생성 시점 보기
	//BALOG(Warning, TEXT("PostLogin Begin"))
	Super::PostLogin(NewPlayer);
	//BALOG(Warning, TEXT("PostLogin End"))

	// PlayerState 초기화
	auto BAPlayerState = Cast<ABAPlayerState>(NewPlayer->PlayerState);
	BACHECK(nullptr != BAPlayerState);
	BAPlayerState->InitPlayerData();
}


void ABAGameMode::AddScore(ABAPlayerController* ScoredPlayer)
{
	// 플레이중인 플레이어의 컨트롤러를 전부 가져와서 막타를 친 (ScoredPlayer)에게만 개인 점수 부여
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<ABAPlayerController>(It->Get());			// 컨트롤러 가져오기
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();											// 해당 컨트롤러에 점수 올리기
			break;
		}
	}

	BAGameState->AddGameScore();			// 전체 스코어 올림


	// 클리어 점수 이상이면
	if (BAGameState->GetTotalGameScore() >= ScoreToClear)
	{
		BAGameState->SetGameCleared();

		for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
		{
			(*It)->TurnOff();
		}

		for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
		{
			const auto ABPlayerController = Cast<ABAPlayerController>(It->Get());
			if (nullptr != ABPlayerController)
			{
				ABPlayerController->ShowResultUI();													// 결과화면 띄우기
			}
		}
	}
}

int32 ABAGameMode::GetTotalScore() const
{
	return BAGameState->GetTotalGameScore();
}
