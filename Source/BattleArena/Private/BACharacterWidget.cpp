// Fill out your copyright notice in the Description page of Project Settings.


#include "BACharacterWidget.h"
#include "BACharacterStatComponent.h"
#include <UMG/Public/Components/ProgressBar.h>		// UProgressBar

void UBACharacterWidget::BindCharacterStat(UBACharacterStatComponent* NewCharacterStat)
{
	BACHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UBACharacterWidget::UpdateHPWidget);		// 약포인트 연결시 AddUObject 사용


	//// 정상 바인딩 되었는지 테스트용
	//NewCharacterStat->OnHPChanged.AddLambda([this]()-> void {
	//	if (CurrentCharacterStat.IsValid())
	//	{
	//		BALOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
	//	}
	//});

}

// begineplay에서 UI생성이 되므로 Postinitial 함수명령은 반영X, NativeConstruct 반영된다
void UBACharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));			// PB_HPBar 이름 찾아넣기
	BACHECK(nullptr != HPProgressBar);
	UpdateHPWidget();
}

void UBACharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
