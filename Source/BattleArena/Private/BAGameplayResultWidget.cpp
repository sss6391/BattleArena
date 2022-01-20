// Fill out your copyright notice in the Description page of Project Settings.


#include "BAGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "BAGameState.h"

// UI ������ NativeCOnstruct �Լ��� AddToViewport �Լ��� �ܺο��� ȣ��� �� UI ������ �ʱ�ȭ �Ǹ鼭 ȣ��ȴٴ� Ư¡
// �÷��̾� ��Ʈ�ѷ��� ShowResultUI �Լ����� AddToViewport �Լ��� ȣ���ϱ� ���� �̸� UI ������ ���ӽ�����Ʈ�� ������ �о���� �� �ֵ��� ���ε��� ����
void UBAGameplayResultWidget::BindGameState(ABAGameState* GameState)
{
	BACHECK(nullptr != GameState);
	CurrentGameState = GameState;
}

void UBAGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	BACHECK(CurrentGameState.IsValid());

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	BACHECK(nullptr != Result);
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ?
		TEXT("Mission Complte") : TEXT("Mission Failed")));

	auto TotalScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTotalScore")));
	BACHECK(nullptr != TotalScore);
	TotalScore->SetText(FText::FromString(FString::FromInt(CurrentGameState->GetTotalGameScore())));
}
