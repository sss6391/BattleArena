// Fill out your copyright notice in the Description page of Project Settings.


#include "BAGameplayWidget.h"
#include "Components/Button.h"
#include "BAPlayerController.h"
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics 사용을 위해

// UI 위젯의 NativeCOnstruct 함수는 AddToViewport 함수가 외부에서 호출될 떄 UI 위젯이 초기화 되면서 호출된다는 특징
void UBAGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼과 함수 바인딩
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

	RemoveFromParent();														// 위젯 제거
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
