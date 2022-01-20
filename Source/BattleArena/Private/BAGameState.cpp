// ���Ӹ�忡�� �����Ѵ�.

#include "BAGameState.h"

ABAGameState::ABAGameState()
{
	TotalGameScore = 0;
	bGameCleared = false;
}

int32 ABAGameState::GetTotalGameScore() const
{
	return TotalGameScore;
}

void ABAGameState::AddGameScore()
{
	TotalGameScore++;
}

void ABAGameState::SetGameCleared()
{
	bGameCleared = true;
}

bool ABAGameState::IsGameCleared() const
{
	return bGameCleared;
}
