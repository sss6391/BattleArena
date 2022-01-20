// Fill out your copyright notice in the Description page of Project Settings.


#include "BAGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "BAGameState.h"

// UI 위젯의 NativeCOnstruct 함수는 AddToViewport 함수가 외부에서 호출될 떄 UI 위젯이 초기화 되면서 호출된다는 특징
// 플레이어 컨트롤러의 ShowResultUI 함수에서 AddToViewport 함수를 호출하기 전에 미리 UI 위젯이 게임스테이트의 정보를 읽어들일 수 있도록 바인딩을 설정
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
