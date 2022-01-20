// UI_Select의 부모 클래스


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
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics 사용을 위해

void UBACharacterSelectWidget::NextCharacter(bool bForward)
{
	bForward ? CurrentIndex++ : CurrentIndex--;

	if (CurrentIndex == -1) CurrentIndex = MaxIndex - 1;
	if (CurrentIndex == MaxIndex) CurrentIndex = 0;

	auto CharacterSetting = GetDefault<UBACharacterSetting>();
	auto AssetRef = CharacterSetting->CharacterAssets[CurrentIndex];			// 캐릭터 에셋 불러오기

	auto ABGameInstance = GetWorld()->GetGameInstance<UBAGameInstance>();
	BACHECK(nullptr != ABGameInstance);
	BACHECK(TargetComponent.IsValid());

	USkeletalMesh* Asset = ABGameInstance->StreamableManager.LoadSynchronous<USkeletalMesh>(AssetRef);			// 동기 에셋로딩
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

	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)				// 월드에서 ASkeletalMeshActor에 해당하는 모든 엑터 목록을 가져옴
	//for (const auto& entity : TActorRange<AStaticMeshActor>(world))				// 동일방식
	{
		TargetComponent = It->GetSkeletalMeshComponent();							
		break;																		// 맨처음꺼만 읽음
	}
	
	// 각종 버튼과 직접 바인딩
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
	if (UGameplayStatics::SaveGameToSlot(NewPlayerData, BAPlayerState->SaveSlotName, 0))				// 게임을 저장하고 다음 레벨을 열면서 저장된 게임을 로드해서 데이터를 넘긴다
	{
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("GamePlay"));
	}
	else
	{
		BALOG(Error, TEXT("SaveGame Error!"));
	}
}
