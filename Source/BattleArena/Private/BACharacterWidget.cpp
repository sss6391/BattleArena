// Fill out your copyright notice in the Description page of Project Settings.


#include "BACharacterWidget.h"
#include "BACharacterStatComponent.h"
#include <UMG/Public/Components/ProgressBar.h>		// UProgressBar

void UBACharacterWidget::BindCharacterStat(UBACharacterStatComponent* NewCharacterStat)
{
	BACHECK(nullptr != NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UBACharacterWidget::UpdateHPWidget);		// ������Ʈ ����� AddUObject ���


	//// ���� ���ε� �Ǿ����� �׽�Ʈ��
	//NewCharacterStat->OnHPChanged.AddLambda([this]()-> void {
	//	if (CurrentCharacterStat.IsValid())
	//	{
	//		BALOG(Warning, TEXT("HPRatio : %f"), CurrentCharacterStat->GetHPRatio());
	//	}
	//});

}

// begineplay���� UI������ �ǹǷ� Postinitial �Լ������ �ݿ�X, NativeConstruct �ݿ��ȴ�
void UBACharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));			// PB_HPBar �̸� ã�Ƴֱ�
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
