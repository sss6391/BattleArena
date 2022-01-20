// HUB위젯 생성, 위젯과 PlayerState 바인드

#include "BAPlayerController.h"
#include "BAHUDWidget.h"
#include "BAPlayerState.h"
#include "BACharacter.h"
#include "BAGameplayWidget.h"
#include "BAGameplayResultWidget.h"
#include "BAGameState.h"
#include <Engine/Classes/Kismet/GameplayStatics.h>		// UGameplayStatics 사용을 위해


ABAPlayerController::ABAPlayerController()
{
	// UI_HUD UI 블루프린트 클래스 불러오기
	static ConstructorHelpers::FClassFinder<UBAHUDWidget> UI_HUD_C(TEXT("/Game/Book/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}

	// ESC누르면 나오는 일시정지 메뉴
	static ConstructorHelpers::FClassFinder<UBAGameplayWidget> UI_MENU_C(TEXT("/Game/Book/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}

	// 게임 종료시 나오는 결과 화면
	static ConstructorHelpers::FClassFinder<UBAGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Book/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}
}

void ABAPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	//BALOG_S(Warning)						// 함수 동작 시기 테스트용
}

void ABAPlayerController::OnPossess(APawn* aPawn)
{
	//BALOG_S(Warning)
	Super::OnPossess(aPawn);

	// 패키징 후에 beginPlay 함수의 실행 순서가 바뀌므로 먼저 실행되는OnPossess 로 옮겨준다(fatal error)
	// 에디터상에서는 컨트롤러의 begin이 먼저 실행되어서 오류가 없으나 패키징시 캐릭터의 begin 먼저 실행됨, BindCharacterStat가 없어서 오류발생
	HUDWidget = CreateWidget<UBAHUDWidget>(this, HUDWidgetClass);
	BACHECK(nullptr != HUDWidget);
	HUDWidget->AddToViewport(1);											// 번호 1로

	// 결과화면
	ResultWidget = CreateWidget<UBAGameplayResultWidget>(this, ResultWidgetClass);
	BACHECK(nullptr != ResultWidget);

}

void ABAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// UI를 배제하고 키 입력이 들어가도록 함
	//FInputModeGameOnly InputGameOnlyMode;
	//SetInputMode(InputGameOnlyMode);
	ChangeInputMode(true);

	// OnPossess 함수로 이동됨
	// HUD위젯 생성후 뷰포트에 추가, 붙이는 방식은 SetWidgetClass로 붙임
	//HUDWidget = CreateWidget<UBAHUDWidget>(this, HUDWidgetClass);
	//BACHECK(nullptr != HUDWidget);
	//HUDWidget->AddToViewport(1);											
	// 
	//// 결과화면
	//ResultWidget = CreateWidget<UBAGameplayResultWidget>(this, ResultWidgetClass);
	//BACHECK(nullptr != ResultWidget);
	////////

	// 컨트롤러에 기본적으로 지정된 PlayerState, HUDWidget에 BAPlayerState 포인트 연결 (중간 역할)
	BAPlayerState = Cast<ABAPlayerState>(PlayerState);
	BACHECK(nullptr != BAPlayerState);
	HUDWidget->BindPlayerState(BAPlayerState);				// 생성된 HUD위젯에 PlayerState 포인트 연결
	BAPlayerState->OnPlayerStateChanged.Broadcast();		// 초기값 브로드 캐스트 시켜서 HUD 세팅
}


UBAHUDWidget* ABAPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}

// ABACharacter에서 죽으면 호출함, takedamage 함수에서 컨트롤러를 넘기기 때문
void ABAPlayerController::NPCKill(ABACharacter* KilledNPC) const
{
	bool DidLevelUp = BAPlayerState->AddExp(KilledNPC->GetExp());		// 경험치 적용
	
	// 레벨업을 하면 레벨업 정보를 state에게 넘김
	if (DidLevelUp)
	{
		auto CurrentPlayer = Cast<ABACharacter>(GetCharacter());
		//auto CurrentPlayer = Cast<ABACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		BACHECK(nullptr != CurrentPlayer);
		CurrentPlayer->CharacterStat->SetNewLevel(BAPlayerState->GetCharacterLevel());
	}
}

// 게임모드에서 호출
void ABAPlayerController::AddGameScore() const
{
	BAPlayerState->AddGameScore();					// 개인 스테이트에 점수 올리기, 브로드캐스트
}

void ABAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// 게임 Pause 키 바인드
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &ABAPlayerController::OnGamePause);
}

// true 이면 게임모드, false면 UI모드
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
	// UI 위젯의 NativeCOnstruct 함수는 AddToViewport 함수가 외부에서 호출될 떄 UI 위젯이 초기화 되면서 호출된다는 특징 때문에  
	// UI 위젯이 게임스테이트의 정보를 읽어들일 수 있도록 바인딩을 설정
	auto ABGameState = Cast<ABAGameState>(UGameplayStatics::GetGameState(this));			// 현제 게임의 게임스테이트를 가져옴
	BACHECK(nullptr != ABGameState);
	ResultWidget->BindGameState(ABGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}

void ABAPlayerController::OnGamePause()
{
	// Pause 메뉴 위젯 띄우기
	MenuWidget = CreateWidget<UBAGameplayWidget>(this, MenuWidgetClass);
	BACHECK(nullptr != MenuWidget);
	MenuWidget->AddToViewport(3);											// 3번포트

	SetPause(true);
	ChangeInputMode(false);
}
