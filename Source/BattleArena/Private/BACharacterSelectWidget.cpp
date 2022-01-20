// UI_Select�� �θ� Ŭ����


#include "BACharacterSelectWidget.h"
#include "BACharacterSelectWidget.h"
#include "BACharacterSetting.h"
#include "BAGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "BASaveGame.h"
#include "BAPlayerState.h"
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics ����� ����

void UBACharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UBACharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];			// ĳ���� ���� �ҷ�����

	auto ABGameInstance = GetWorld()->GetGameInstance<UBAGameInstance>();
	BACHECK(nullptr != ABGameInstance);
	BACHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);			// ���� ���·ε�
	if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}
}

void UBACharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UBACharacterSetting>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)				// ���忡�� ASkeletalMeshActor�� �ش��ϴ� ��� ���� ����� ������
	//for (const auto& entity : TActorRange<AStaticMeshActor>(world))				// ���Ϲ��
	{
		TargetComponent = It->GetSkeletalMeshComponent();							
		break;																		// ��ó������ ����
	}
	
	// ���� ��ư�� ���� ���ε�
	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	BACHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	BACHECK(nullptr != NextButton);

	TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	BACHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	BACHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UBACharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UBACharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UBACharacterSelectWidget::OnConfirmClicked);
	////
}

void UBACharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UBACharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UBACharacterSelectWidget::OnConfirmClicked()
{
	FString CharacterName = TextBox->GetText().ToString();
	if (CharacterName.Len() <= 0 || CharacterName.Len() > 10) return;

	UBASaveGame* NewPlayerData = NewObject<UBASaveGame>();
	NewPlayerData->PlayerName = CharacterName;
	NewPlayerData->Level = 1;
	NewPlayerData->Exp = 0;
	NewPlayerData->HighScore = 0;
	NewPlayerData->CharacterIndex = CurrentIndex;

	auto BAPlayerState = GetDefault<ABAPlayerState>();
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, BAPlayerState->SaveSlotName, 0))				// ������ �����ϰ� ���� ������ ���鼭 ����� ������ �ε��ؼ� �����͸� �ѱ��
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		BALOG(Error, TEXT("SaveGame Error!"));
	}
}
