// Fill out your copyright notice in the Description page of Project Settings.


#include "BAGameplayWidget.h"
#include "Components/Button.h"
#include "BAPlayerController.h"
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics ����� ����

// UI ������ NativeCOnstruct �Լ��� AddToViewport �Լ��� �ܺο��� ȣ��� �� UI ������ �ʱ�ȭ �Ǹ鼭 ȣ��ȴٴ� Ư¡
void UBAGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ��ư�� �Լ� ���ε�
	ResumeButton = Cast<UButton>(GetWidgetFromName(TEXT("btnResume")));
	if (nullptr != ResumeButton)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UBAGameplayWidget::OnResumeClicked);
	}

	ReturnToTitleButton = Cast<UButton>(GetWidgetFromName(TEXT("btnReturnToTitle")));
	if (nullptr != ReturnToTitleButton)
	{
		ReturnToTitleButton->OnClicked.AddDynamic(this, &UBAGameplayWidget::OnReturnToTitleClicked);
	}

	RetryGameButton = Cast<UButton>(GetWidgetFromName(TEXT("btnRetryGame")));
	if (nullptr != RetryGameButton)
	{
		RetryGameButton->OnClicked.AddDynamic(this, &UBAGameplayWidget::OnRetryGameClicked);
	}
}

void UBAGameplayWidget::OnResumeClicked()
{
	auto ABPlayerController = Cast<ABAPlayerController>(GetOwningPlayer());
	BACHECK(nullptr != ABPlayerController);

	RemoveFromParent();														// ���� ����
	ABPlayerController->ChangeInputMode(true);
	ABPlayerController->SetPause(false);
}

void UBAGameplayWidget::OnReturnToTitleClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Title"));
}

void UBAGameplayWidget::OnRetryGameClicked()
{
	auto BAPlayerController = Cast<ABAPlayerController>(GetOwningPlayer());
	BACHECK(nullptr != BAPlayerController);
	BAPlayerController->RestartLevel();
}
