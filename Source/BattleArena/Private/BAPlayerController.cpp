// HUB���� ����, ������ PlayerState ���ε�

#include "BAPlayerController.h"
#include "BAHUDWidget.h"
#include "BAPlayerState.h"
#include "BACharacter.h"
#include "BAGameplayWidget.h"
#include "BAGameplayResultWidget.h"
#include "BAGameState.h"
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics ����� ����


ABAPlayerController::ABAPlayerController()
{
	// UI_HUD UI �������Ʈ Ŭ���� �ҷ�����
	static ConstructorHelpers::FClassFinder<UBAHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// ESC������ ������ �Ͻ����� �޴�
	static ConstructorHelpers::FClassFinder<UBAGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	// ���� ����� ������ ��� ȭ��
	static ConstructorHelpers::FClassFinder<UBAGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void ABAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//BALOG_S(Warning)						// �Լ� ���� �ñ� �׽�Ʈ��
}

void ABAPlayerController::OnPossess(APawn* aPawn)
{
	//BALOG_S(Warning)
	Super::OnPossess(aPawn);

	// ��Ű¡ �Ŀ� beginPlay �Լ��� ���� ������ �ٲ�Ƿ� ���� ����Ǵ�OnPossess �� �Ű��ش�(fatal error)
	// �����ͻ󿡼��� ��Ʈ�ѷ��� begin�� ���� ����Ǿ ������ ������ ��Ű¡�� ĳ������ begin ���� �����, BindCharacterStat�� ��� �����߻�
	HUDWidget = CreateWidget<UBAHUDWidget>(this, HUDWidgetClass);
	BACHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);											// ��ȣ 1��

	// ���ȭ��
	ResultWidget = CreateWidget<UBAGameplayResultWidget>(this, ResultWidgetClass);
	BACHECK(nullptr != ResultWidget);

}

void ABAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI�� �����ϰ� Ű �Է��� ������ ��
	//FInputModeGameOnly InputGameOnlyMode;
	//SetInputMode(InputGameOnlyMode);
	ChangeInputMode(true);

	// OnPossess �Լ��� �̵���
	// HUD���� ������ ����Ʈ�� �߰�, ���̴� ����� SetWidgetClass�� ����
	//HUDWidget = CreateWidget<UBAHUDWidget>(this, HUDWidgetClass);
	//BACHECK(nullptr != HUDWidget);
	//HUDWidget->AddToViewport(1);											
	// 
	//// ���ȭ��
	//ResultWidget = CreateWidget<UBAGameplayResultWidget>(this, ResultWidgetClass);
	//BACHECK(nullptr != ResultWidget);
	////////

	// ��Ʈ�ѷ��� �⺻������ ������ PlayerState, HUDWidget�� BAPlayerState ����Ʈ ���� (�߰� ����)
	BAPlayerState = Cast<ABAPlayerState>(PlayerState);
	BACHECK(nullptr != BAPlayerState);
	HUDWidget->BindPlayerState(BAPlayerState);				// ������ HUD������ PlayerState ����Ʈ ����
	BAPlayerState->OnPlayerStateChanged.Broadcast();		// �ʱⰪ ��ε� ĳ��Ʈ ���Ѽ� HUD ����
}


UBAHUDWidget* ABAPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// ABACharacter���� ������ ȣ����, takedamage �Լ����� ��Ʈ�ѷ��� �ѱ�� ����
void ABAPlayerController::NPCKill(ABACharacter* KilledNPC) const
{
	bool DidLevelUp = BAPlayerState->AddExp(KilledNPC->GetExp());		// ����ġ ����
	
	// �������� �ϸ� ������ ������ state���� �ѱ�
	if (DidLevelUp)
	{
		auto CurrentPlayer = Cast<ABACharacter>(GetCharacter());
		//auto CurrentPlayer = Cast<ABACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		BACHECK(nullptr != CurrentPlayer);
		CurrentPlayer->CharacterStat->SetNewLevel(BAPlayerState->GetCharacterLevel());
	}
}

// ���Ӹ�忡�� ȣ��
void ABAPlayerController::AddGameScore() const
{
	BAPlayerState->AddGameScore();					// ���� ������Ʈ�� ���� �ø���, ��ε�ĳ��Ʈ
}

void ABAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// ���� Pause Ű ���ε�
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ABAPlayerController::OnGamePause);
}

// true �̸� ���Ӹ��, false�� UI���
void ABAPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}

void ABAPlayerController::ShowResultUI()
{
	// UI ������ NativeCOnstruct �Լ��� AddToViewport �Լ��� �ܺο��� ȣ��� �� UI ������ �ʱ�ȭ �Ǹ鼭 ȣ��ȴٴ� Ư¡ ������  
	// UI ������ ���ӽ�����Ʈ�� ������ �о���� �� �ֵ��� ���ε��� ����
	auto ABGameState = Cast<ABAGameState>(UGameplayStatics::GetGameState(this));			// ���� ������ ���ӽ�����Ʈ�� ������
	BACHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void ABAPlayerController::OnGamePause()
{
	// Pause �޴� ���� ����
	MenuWidget = CreateWidget<UBAGameplayWidget>(this, MenuWidgetClass);
	BACHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);											// 3����Ʈ

	SetPause(true);
	ChangeInputMode(false);
}
