// BAPlayerController ���� ���������Ͽ� ȭ�鿡 ���

#include "BAHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "BAPlayerState.h"
#include "BACharacterStatComponent.h"


// ĳ���� ������Ʈ OnHPChanged ��������Ʈ ���ε�, ĳ���Ϳ��� ȣ��
void UBAHUDWidget::BindCharacterStat(UBACharacterStatComponent* CharacterStat)
{
	BACHECK(nullptr != CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UBAHUDWidget::UpdateCharacterStat);
}

// �÷��̾� ������Ʈ OnPlayerStateChanged ��������Ʈ ���ε�, ��Ʈ�ѷ����� ȣ��
void UBAHUDWidget::BindPlayerState(ABAPlayerState* PlayerState)
{
	BACHECK(nullptr != PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UBAHUDWidget::UpdatePlayerState);
}

void UBAHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// UI ���ε�
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

// PlayerState ���� ó�� �����͸� UI�� ����
void UBAHUDWidget::UpdatePlayerState()
{
	BACHECK(CurrentPlayerState.IsValid());

	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	HighScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameHighScore())));
}
