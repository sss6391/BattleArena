// ������ �⺻ Ŭ���� ����, �ʱ�ȭ, Ŭ����� ����

#include "BAGameMode.h"
//#include "BAPawn.h"				// �� �⺻���� (�׽�Ʈ��)
#include "BACharacter.h"			// ��
#include "BAPlayerController.h"		// ��Ʈ�ѷ� �⺻
#include "BAPlayerState.h"			// ���� �⺻
#include "BAGameState.h"			// ���� ���� ����

ABAGameMode::ABAGameMode()
{
	//DefaultPawnClass = ABAPawn::StaticClass();
	DefaultPawnClass = ABACharacter::StaticClass();					// �� Ŭ���� ����
	PlayerControllerClass = ABAPlayerController::StaticClass();		// ��Ʈ�ѷ� Ŭ���� ����
	PlayerStateClass = ABAPlayerState::StaticClass();				// ���� Ŭ���� ����
	GameStateClass = ABAGameState::StaticClass();					// ���� ������Ʈ Ŭ���� ����

	ScoreToClear = 10;				// Ŭ���� ���� ����
}

void ABAGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BAGameState = Cast<ABAGameState>(GameState);		// ������ ���ӽ�����Ʈ�� ������
}

// �÷��̾� ��Ʈ�ѷ� ����-> �÷��̾� �� ����-> ���� -> ���ӽ���
// ������Ʈ ����-> GameMode::PostLogin Begin ->Pawn ����->����->GameMode::PostLogin End ����
void ABAGameMode::PostLogin(APlayerController* NewPlayer)
{
	// ���� ���� ����
	//BALOG(Warning, TEXT("PostLogin Begin"))
	Super::PostLogin(NewPlayer);
	//BALOG(Warning, TEXT("PostLogin End"))

	// PlayerState �ʱ�ȭ
	auto BAPlayerState = Cast<ABAPlayerState>(NewPlayer->PlayerState);
	BACHECK(nullptr != BAPlayerState);
	BAPlayerState->InitPlayerData();
}


void ABAGameMode::AddScore(ABAPlayerController* ScoredPlayer)
{
	// �÷������� �÷��̾��� ��Ʈ�ѷ��� ���� �����ͼ� ��Ÿ�� ģ (ScoredPlayer)���Ը� ���� ���� �ο�
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		const auto ABPlayerController = Cast<ABAPlayerController>(It->Get());			// ��Ʈ�ѷ� ��������
		if ((nullptr != ABPlayerController) && (ScoredPlayer == ABPlayerController))
		{
			ABPlayerController->AddGameScore();											// �ش� ��Ʈ�ѷ��� ���� �ø���
			break;
		}
	}

	BAGameState->AddGameScore();			// ��ü ���ھ� �ø�


	// Ŭ���� ���� �̻��̸�
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
				ABPlayerController->ShowResultUI();													// ���ȭ�� ����
			}
		}
	}
}

int32 ABAGameMode::GetTotalScore() const
{
	return BAGameState->GetTotalGameScore();
}
