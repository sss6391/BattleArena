// BAPlayerController 에서 위젯생성하여 화면에 띄움

#include "BAHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BAPlayerState.h"
#include "BACharacterStatComponent.h"


// 캐릭터 스테이트 OnHPChanged 델리게이트 바인드, 캐릭터에서 호출
void UBAHUDWidget::BindCharacterStat(UBACharacterStatComponent* CharacterStat)
{
	BACHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UBAHUDWidget::UpdateCharacterStat);
}

// 플레이어 스테이트 OnPlayerStateChanged 델리게이트 바인드, 컨트롤러에서 호출
void UBAHUDWidget::BindPlayerState(ABAPlayerState* PlayerState)
{
	BACHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UBAHUDWidget::UpdatePlayerState);
}

void UBAHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// UI 바인드
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	BACHECK(nullptr != HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbExp")));
	BACHECK(nullptr != ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	BACHECK(nullptr != PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	BACHECK(nullptr != PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	BACHECK(nullptr != CurrentScore);

	HighScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtHighScore")));
	BACHECK(nullptr != HighScore);
}

void UBAHUDWidget::UpdateCharacterStat()
{
	BACHECK(CurrentCharacterStat.IsValid());

	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

// PlayerState 에서 처리 데이터를 UI에 적용
void UBAHUDWidget::UpdatePlayerState()
{
	BACHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
}
