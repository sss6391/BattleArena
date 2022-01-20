// Fill out your copyright notice in the Description page of Project Settings.


#include "BACharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"			// UWidgetComponent 사용을 위해
#include "BACharacterWidget.h"					// HP 위젯 부모
#include "BAAIController.h"
#include "BAPlayerController.h"
#include "BACharacterSetting.h"					// 다른 모듈인 BACharacterSetting에서 읽은 에셋을 사용하기 위해
#include "BAPlayerState.h"						// ABAPlayerState, BAGameMode에서 기본으로 지정
#include "BAGameInstance.h"						// FStreamableManager가 선언되어 비동기 에셋로딩을 함
//#include <Engine/AssetManager.h>				// FStreamableManager가 엔진모듈에 선언되어있다. 이 헤더 선언후 UAssetManager::GetStreamableManager() 함수를 사용하여도 됨 //UAssetManager::GetStreamableManager().RequestAsyncLoad()
#include "BAHUDWidget.h"
#include "BAGameMode.h"							// 점수에 따른 NPC 레벨설정을 위해


// Sets default values
ABACharacter::ABACharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	CharacterStat = CreateDefaultSubobject<UBACharacterStatComponent>(TEXT("CHRACTERSTAT"));
	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("BPBARWIDGET"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	HPBarWidget->SetupAttachment(GetMesh());

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));


	// Skeletal Mesh를 카보드맨으로 지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
		SK_CARDBOARD(TEXT("/Game/InfinityBladeWarriors/Character/CompleteCharacters/SK_CharM_Cardboard.SK_CharM_Cardboard"));
	if (SK_CARDBOARD.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_CARDBOARD.Object);
	}

	// 애님 블루프린트 클래스를 인스턴스 속성에 지정(_C를 붙임) 
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance>
		WARRIOR_ANIM(TEXT("/Game/Animations/WarriorAnimBlueprint.WarriorAnimBlueprint_C"));
	if (WARRIOR_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(WARRIOR_ANIM.Class);
	}

	//// 오른손에 무기를 고정적으로 붙여줌
	//FName WeaponSocket(TEXT("hand_rSocket"));
	//if (GetMesh()->DoesSocketExist(WeaponSocket))
	//{
	//	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	//	static ConstructorHelpers::FObjectFinder<USkeletalMesh>
	//		SK_WEAPON(TEXT("SkeletalMesh'/Game/InfinityBladeWeapons/Weapons/Blade/Swords/Blade_BlackKnight/SK_Blade_BlackKnight.SK_Blade_BlackKnight'"));
	//	if (SK_WEAPON.Succeeded())
	//	{
	//		Weapon->SetSkeletalMesh(SK_WEAPON.Object);			// 불러온 메쉬를 컴포넌트에 설정
	//		Weapon->SetupAttachment(GetMesh(), WeaponSocket);	// 메쉬 컴포넌트를 소켓에 설정
	//	}
	//}
	//////////////////////////////////////////


	// HP바 위젯 설정
	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);					// 위젯이 항상 화면을 향함
	// 만든어둔 블루프린트 UI 위젯 넣기 
	static ConstructorHelpers::FClassFinder<UUserWidget>
		UI_HUD(TEXT("/Game/Book/UI/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}


	//SetControlMode(0);
	//SetControlMode(EControlMode::GTA);
	SetControlMode(EControlMode::DIABLO);

	// 컨트롤 모드 전환시 카메라 속도
	fArmLengthSpeed = 3.0f;
	fArmRotationSpeed = 5.0f;

	// 점프 높이 조절
	//GetCharacterMovement()->JumpZVelocity = 800.0f;		// 기존
	GetCharacterMovement()->JumpZVelocity = 400.0f;

	// 델리게이트 확인
	IsAttacking = false;

	// 콤보설정
	MaxCombo = 4;
	AttackEndComboState();

	// 콜리전프리셋 설정
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("BACharacter"));

	AttackRange = 80.0f;	// 디버그 원통 길이, 기본 공격범위
	AttackRadius = 50.0f;	//디버그 원 반지름

	// AI컨트롤러 설정
	AIControllerClass = ABAAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;			// 스폰시 자동으로 AI컨트롤러 배치


	// 테스트로 에셋이 로드 됐는지 확인하기, DefaultBattleArena.ini
	/*auto DefaultSetting = GetDefault<UBACharacterSetting>();
	if (DefaultSetting->CharacterAssets.Num() > 0)
	{
		for (auto CharaterAsset : DefaultSetting->CharacterAssets)
		{
			BALOG(Warning, TEXT("Character Asset : %s"), *CharaterAsset.ToString());
		}
	}
	else
	{
		BALOG(Warning, TEXT("캐릭터 에셋로드 실패"));
	}
	*//////////////

	// 상태관련
	SetActorHiddenInGame(true);			
	HPBarWidget->SetHiddenInGame(true);
	SetCanBeDamaged(false);

	DeadTimer = 3;
}


void ABACharacter::SetCharacterState(ECharacterState NewState)
{
	BACHECK(CurrentState != NewState);
	CurrentState = NewState;

	switch (NewState)
	{
	// 로딩 상태일때 모든걸 숨긴다
	case ECharacterState::LOADING:
	{
		if (bIsPlayer)
		{
			DisableInput(BAPlayerController);

			// CharacterStat를 컨트롤러에 있는 HUDWidget에 포인트 연결(델리게이트 바인드됨), 플레이어만 HUD 사용함
			BAPlayerController->GetHUDWidget()->BindCharacterStat(CharacterStat);			// 바인딩시 컨트롤러의 HUDWidget 이 생성되어 있어야 하는데 없으면 fatal error

			// ABAPlayerState 스테이트 설정, 게임모드에서 설정한 스테이트를 가져옴 (PlayerState)
			auto BAPlayerState = Cast<ABAPlayerState>(GetPlayerState());
			BACHECK(nullptr != BAPlayerState);
			CharacterStat->SetNewLevel(BAPlayerState->GetCharacterLevel());			// PlayerState에서 설정한 레벨을 데이터 테이블에서 해당 레벨 적용 (PlayerState에서 저장된 레벨 가져옴)
		}
		else
		{
			auto BAGameMode = Cast<ABAGameMode>(GetWorld()->GetAuthGameMode());					// 게임모드는 멀티플레이 겜에서 게임을 관리하는 방장역할
			BACHECK(nullptr != BAGameMode);
			int32 TargetLevel = FMath::CeilToInt(((float)BAGameMode->GetTotalScore() * 0.8f));		// 전체 스코어를 기반으로 NPC 레벨계산
			int32 FinalLevel = FMath::Clamp<int32>(TargetLevel, 1, 20);
			BALOG(Warning, TEXT("New NPC Level : %d"), FinalLevel);
			CharacterStat->SetNewLevel(FinalLevel);
		}

		SetActorHiddenInGame(true);
		HPBarWidget->SetHiddenInGame(true);
		SetCanBeDamaged(false);
		break;
	}
	// 준비가 완료되면 모든 모습을 보인다
	case ECharacterState::READY:
	{
		SetActorHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(false);
		SetCanBeDamaged(true);

		// CharacterStat 델리게이트 연결, CharacterStat에서는 HP만 관리중 (NPC의 경우 HP만 있으면 되기 때문)
		CharacterStat->OnHPIsZero.AddLambda([this]()->void {SetCharacterState(ECharacterState::DEAD); });

		// UI 위젯 델리게이트 바인딩 함수 호출  (NPC의 경우 HP 위젯만 보이면 됨)
		auto CharacterWidget = Cast<UBACharacterWidget>(HPBarWidget->GetUserWidgetObject());			// HPBarWidget 는 UWidgetComponent 로 생성되어 블르브린트 클래스만 사용중
		BACHECK(nullptr != CharacterWidget);
		CharacterWidget->BindCharacterStat(CharacterStat);												// switch case 안에 중괄호가 없으면 빌드 오류남

		if (bIsPlayer)
		{
			SetControlMode(EControlMode::DIABLO);
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
			EnableInput(BAPlayerController);

			// 테스트 캐릭터 위 HP바 숨기기, 잘됨
			//HPBarWidget->SetHiddenInGame(true);
		}
		else
		{
			SetControlMode(EControlMode::NPC);
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
			BAAIController->RunAI();						// AI 수동으로 시작되게
		}
		break;
	}
	case ECharacterState::DEAD:
	{
		SetActorEnableCollision(false);
		GetMesh()->SetHiddenInGame(false);
		HPBarWidget->SetHiddenInGame(true);
		BAAnim->SetDeadAnim();
		SetCanBeDamaged(false);

		if (bIsPlayer)
		{
			DisableInput(BAPlayerController);
		}
		else
		{
			BAAIController->StopAI();
		}

		// 죽으면 타이머 동작후 재시작또는 파괴
		GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			if (bIsPlayer)
			{
				//BAPlayerController->RestartLevel();				// 플레이어가 죽으면 레벨 재시작 (자체함수)
				BAPlayerController->ShowResultUI();				// 결과 UI 띄움
			}
			else
			{
				Destroy();
			}
			}), DeadTimer, false);
		break;
	}
	}
}

ECharacterState ABACharacter::GetCharacterState() const
{
	return CurrentState;
}

// Called when the game starts or when spawned
void ABACharacter::BeginPlay()
{
	Super::BeginPlay();

	// 무기액터 스폰시켜 오른손에 붙이기, 액터스폰은 BeginPlay 에 사용가능 // SetWeapon 함수로 이동됨
	//FName WeaponSocket(TEXT("hand_rSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<ABAWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurWeapon)
	//{
	//	CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
	//}

	// UI 위젯 바인딩
	//auto CharacterWidget = Cast<UBACharacterWidget>(HPBarWidget->GetUserWidgetObject());
	//if (nullptr != CharacterWidget)
	//{
	//	CharacterWidget->BindCharacterStat(CharacterStat);
	//}

	// 비동기 에셋 로딩, NPC의 랜덤 에셋 로딩, 
	//if (!IsPlayerControlled())
	//{
	//	auto DefaultSetting = GetDefault<UBACharacterSetting>();
	//	int32 RandIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);		// 랜덤범위
	//	CharacterAssetToLoad = DefaultSetting->CharacterAssets[RandIndex];
	//	auto ABGameInstance = Cast<UBAGameInstance>(GetGameInstance());
	//	if (nullptr != ABGameInstance) {
	//		AssetStreamingHandle = ABGameInstance->StreamableManager.RequestAsyncLoad(
	//			CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ABACharacter::OnAssetLoadCompleted));     // 비동기 에셋로딩 델리게이트, 완료가 되면 등록한 함수가 호출 된다.
	//	}
	//}
	////

	// 상태 설정
	bIsPlayer = IsPlayerControlled();

	if (bIsPlayer)
	{
		BAPlayerController = Cast<ABAPlayerController>(GetController());
		BACHECK(nullptr != BAPlayerController);
	}
	else
	{
		BAAIController = Cast<ABAAIController>(GetController());
		BACHECK(nullptr != BAAIController);
	}

	auto DefaultSetting = GetDefault<UBACharacterSetting>();

	if (bIsPlayer)
	{
		//AssetIndex = 4;				// 플레이어의 경우 임의로 4번의 에셋을 사용한다
		auto BAPlayerState = Cast<ABAPlayerState>(GetPlayerState());
		BACHECK(nullptr != BAPlayerState);
		AssetIndex = BAPlayerState->GetCharacterIndex();
	}
	else
	{
		AssetIndex = FMath::RandRange(0, DefaultSetting->CharacterAssets.Num() - 1);
	}

	CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
	auto BAGameInstance = Cast<UBAGameInstance>(GetGameInstance());
	BACHECK(nullptr != BAGameInstance)

	// 비동기 에셋로딩, 로딩이 끝나면 ECharacterState::READY 상태로
	AssetStreamingHandle = BAGameInstance->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &ABACharacter::OnAssetLoadCompleted));
	SetCharacterState(ECharacterState::LOADING);		// 로딩 상태로

}

void ABACharacter::SetControlMode(EControlMode NewControlMode)
{
	CurrentControlMode = NewControlMode;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		fArmLengthTo = 450.0f;												// 카메라 전환
		SpringArm->bUsePawnControlRotation = true;							// 폰의 컨트롤러 회전 값을 카메라가 사용
		SpringArm->bInheritPitch = true;									
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;									// 카메라를 좌우 방향을 틀어도 캐릭터의 방향은 그대로 고정
		GetCharacterMovement()->bOrientRotationToMovement = true;			// 움직일때 움직임이 있는 방향으로 캐릭터 회전 (액터의 회전사용)
		GetCharacterMovement()->bUseControllerDesiredRotation = false;		// true시 카메라 보는방향으로 항상 회전 (컨트롤러와 카메라가 회전값을 공유해서)

		//GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);	// 캐릭터 회전시 회전속도 지정(초당변화량)
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 300.0f, 0.0f);
		break;

	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
		fArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;		
		GetCharacterMovement()->bOrientRotationToMovement = false;				// 액터의 회전값을 캐릭터 회전으로 사용하지 않도록
		GetCharacterMovement()->bUseControllerDesiredRotation = true;			// 회전을 컨트롤러에서 얻어옴
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);	// 캐릭터 회전시 회전속도 지정(초당변화량)
		break;

	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void ABACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 모드 변환시 부드러운 카메라 전환
	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, fArmLengthTo, DeltaTime, fArmLengthSpeed);					// 캐릭터와의 원근

	// 카메라의 부드러운 전환 해결됨 (SpringArm->SetRelativeRotation(GetViewRotation());)
	FRotator Controllll;			// test
	//FRotator Controllll = SpringArm->GetRelativeRotation();
	//Controllll = GetViewRotation();
	//BALOG(Warning, TEXT("GetViewRotation: Yaw %f, Pitch %f, Roll %f"), Controllll.Yaw, Controllll.Pitch, Controllll.Roll)

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:	

		Controllll = SpringArm->GetRelativeRotation();
		BALOG(Warning, TEXT("SpringArm: Yaw %f, Pitch %f, Roll %f"), Controllll.Yaw, Controllll.Pitch, Controllll.Roll)

		SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, fArmRotationSpeed));	// 카메라 회전

		// test
		//Controllll = FMath::RInterpTo(ControlRot, ArmRotationTo, DeltaTime, fArmRotationSpeed);
		//BALOG(Warning, TEXT("Yaw %f, Pitch %f, Roll %f"), Controllll.Yaw, Controllll.Pitch, Controllll.Roll)
		//SpringArm->SetRelativeRotation(Controllll);	// 카메라 회전
		////
		break;
	}


	// 모드 전환시 이동설정
	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:	// 디아블로 모드 이동 (컨트롤러에서 받은 뒤에 틱함수 동작)
		if (DirectionToMove.SizeSquared() > 0.0f)	// 캐릭터 방향이 원래로 돌아오는 것을 방지
		{
			// 캐릭터 시선방향과 이동방향 일치  (FRotationMatrix::MakeFromX().Rotator() <-두함수는 같음-> UKismetMathLibrary::MakeRotFromX)
			// 컨트롤러를 회전시킴, 회전 값을 캐릭터 방향과 일치시킴 (GetCharacterMovement()->bUseControllerDesiredRotation = true;)
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			//GetController()->SetControlRotation(UKismetMathLibrary::MakeRotFromX(DirectionToMove));

			AddMovementInput(DirectionToMove);
		}
		break;
	}

}

void ABACharacter::ViewChange()
{
	// test
	//ControlRot = GetController()->GetControlRotation();
	BALOG_S(Warning)

		switch (CurrentControlMode)
		{
		case EControlMode::GTA:				// DIABLO 로 
			SpringArm->SetRelativeRotation(GetViewRotation());							// 카메라 전환을 위해 추가함
			GetController()->SetControlRotation(GetActorRotation());					// 컨트롤러의 회전을 사용하기 때문에(카메라 회전값과 다름), 소스가 없으면 전환시 캐릭터의 방향이 카메라 방향으로 돌아감 			
			SetControlMode(EControlMode::DIABLO);
			break;

		case EControlMode::DIABLO:			// GTA 로 전환시
			GetController()->SetControlRotation(SpringArm->GetRelativeRotation());		// 컨트롤 회전값을 카메라가 사용함, 전환시 캐릭터의 방향을 돌아가지 않게, 해당소스가 없으면 캐릭터가 회전한 뒤쪽으로 카메라가 바로 이동함
			SetControlMode(EControlMode::GTA);
			break;
		}
}

// Called to bind functionality to input
void ABACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("ViewChange"), EInputEvent::IE_Pressed, this, &ABACharacter::ViewChange);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ABACharacter::Jump);					// jump 함수는 캐릭터 컴포넌트에 구현되어 있음
	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &ABACharacter::Attack);				

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &ABACharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &ABACharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ABACharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ABACharacter::Turn);
}

int32 ABACharacter::GetExp() const
{
	return CharacterStat->GetDropExp();
}

void ABACharacter::UpDown(float NewAxisValue)
{
	// 액터 기준 앞
	//AddMovementInput(GetActorForwardVector(), NewAxisValue);

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 컨트롤러의 카메라 기준 앞, X축 (RPG움직임)
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.X = NewAxisValue;
		break;
	}


}

void ABACharacter::LeftRight(float NewAxisValue)
{
	//AddMovementInput(GetActorRightVector(), NewAxisValue);


	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		// 컨트롤러의 카메라 기준 오른쪽,  Y축 (RPG움직임), FRotationMatrix 절대축 기준 회전 값
		//AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;

	case EControlMode::DIABLO:
		DirectionToMove.Y = NewAxisValue;
		break;
	}


}

void ABACharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void ABACharacter::Turn(float NewAxisValue)
{

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}



void ABACharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// 콤보공격을 위한 몽타주 플레이, 커스텀으로 만든 인스턴스 불러옴
	BAAnim = Cast<UBAAnimInstance>(GetMesh()->GetAnimInstance());
	BACHECK(nullptr != BAAnim);			// 검증 변수가 참이어야 오류로그를 남기지 않음

	// 애님 모션 종료 델리케이드 바인딩( OnMontageEnded 종료시 바인딩된 함수 실행)
	BAAnim->OnMontageEnded.AddDynamic(this, &ABACharacter::OnAttackMontageEnded);		// .AddDynamic 인텔리센스에 검색되지 않지만 컴파일됨

	// 람다식을 사용하여 델리게이트에 로직 등록, 애님에서 지정한 시점에 호출됨
	BAAnim->OnNextAttackCheck.AddLambda([this]()->void {

			//BALOG(Warning, TEXT("OnNextAttackCheck"));
			CanNextCombo = false;			

			if (IsComboInputOn)
			{
				AttackStartComboState();
				BAAnim->JumpToAttackMontageSection(CurrentCombo);
			}
		});

	// 데미지 주는 시점 바인딩
	BAAnim->OnAttackHitCheck.AddUObject(this, &ABACharacter::AttackCheck);


	// 상태컴포넌트와 바인딩,   주석처리 SetCharacterState() 함수에서 대체함
	//CharacterStat->OnHPIsZero.AddLambda([this]()->void{

	//	BALOG(Warning, TEXT("OnHpIsZeor"));
	//	BAAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);		// 죽으면 충돌 없게
	//	
	//	});
}

// 마우스 왼쪽클릭과 바인딩 됨
void ABACharacter::Attack()
{
	// 콤보 확인
	if (IsAttacking)
	{
		BACHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
		if (CanNextCombo)		// (개인적인의견으론 현재 상태에선 CanNextCombo 은 없어도 되는게 아닌가)
		{
			IsComboInputOn = true;
		}
	}
	else
	{
		BACHECK(CurrentCombo == 0);
		AttackStartComboState();
		BAAnim->PlayAttackMontage();						
		BAAnim->JumpToAttackMontageSection(CurrentCombo);
		IsAttacking = true;
	}

}

void ABACharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	BACHECK(IsAttacking);
	BACHECK(CurrentCombo > 0);
	IsAttacking = false;
	AttackEndComboState();

	// 공격 모션 끝나는 시점을 다시 브로드캐스트
	OnAttackEnd.Broadcast();

	// 콤보 테스트
	//if (IsComboInputOn)
	//{
	//	AttackStartComboState();
	//	BAAnim->JumpToAttackMontageSection(CurrentCombo);
	//}
	//if (CurrentCombo >= 4)
	//{
	//	IsAttacking = false;
	//	AttackEndComboState();
	//}

}

void ABACharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	BACHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void ABACharacter::AttackEndComboState()
{
	IsComboInputOn = false;		
	CanNextCombo = false;
	CurrentCombo = 0;
}

void ABACharacter::AttackCheck()
{
	float FinalAttackRange = GetFinalAttackRange();			// AttackRange -> FinalAttackRange

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);	// 태그네이밍으로부터, 추적복잡성(디테일한판정사용시), 무시할 엑터

	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		//GetActorLocation() + GetActorForwardVector() * 200.0f,			// 캐릭터 앞 200
		GetActorLocation() + GetActorForwardVector() * FinalAttackRange,	// 캐릭터 앞 계산된 공격범위만큼
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,				// 트레이스채널2에 커스텀으로 등록한 attack트레이스가 등록되어있다. config-DefaultEngine.ini에서 확인가능
		FCollisionShape::MakeSphere(AttackRadius),				// 50 반지름 구체 생성
		Params);
	
	if (bResult)
	{
		// 결과 구조체에 액터가 있으면 ( Actor은 약포인터라서 Actor를 사용하기위해 IsValid 로 점검을 하고 써야한다 )
		if (HitResult.Actor.IsValid())
		{
			//BALOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());

			// 데미지 전달 (데미지크기, 종류, 가해자, 사용한 도구)
			FDamageEvent DamageEvent;
			//HitResult.Actor->TakeDamage(50.0f, DamageEvent, GetController(), this);							// 단순 50데미지
			//HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);		// 데이터 테이블에 따른 데미지

			// 최종데미지는 계산후에 적용
			//HitResult.Actor->TakeDamage(GetFinalAttackDamage(), DamageEvent, GetController(), this);			// 데이터 테이블과 무기에 따른 랜덤 데미지

			// 테스트 콤보에 따른 데미지 증가 테스트, 기존 소스 주석처리
			float NewDamage = GetFinalAttackDamage();
			NewDamage = CurrentCombo == 4 ? NewDamage * 2 : NewDamage;
			HitResult.Actor->TakeDamage(NewDamage, DamageEvent, GetController(), this);
		}
	}


	// 디버그용 히트범위표시 
#ifdef ENABLE_DRAW_DEBUG

	//FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector TraceVec = GetActorForwardVector() * FinalAttackRange;				// AttackRange -> FinalAttackRange
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	//float HalfHeight = AttackRange * 0.5f + AttackRadius;
	float HalfHeight = FinalAttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();			// 캐릭터 시선인 Z축 방향으로 일치시킴(시야 앞쪽)
	FColor DrawColor = bResult ? FColor::Red : FColor::Green;					// 맞으면 빨간색으로 표시함
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);
#endif



}

// 데미지 처리(인스턴스의 Can be Damage 설정을 false로 하면 데미지가 0으로 나온다)
float ABACharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//BALOG(Warning, TEXT("액터 : %s  받은 데미지 : %f"), *GetName(), FinalDamage);

	// 받는 데미지 적용
	CharacterStat->SetDamage(FinalDamage);

	// 데미지를 가할때마다 Instigator 로 검사하여 마지막죽음을 기록하는 것은 비효율적이다. 제거될때 LastHitBy 속성에 저장되므로 이를 사용
	if (CurrentState == ECharacterState::DEAD)
	{
		if (EventInstigator->IsPlayerController())								// 데미지를 가한 대상이 플레이어라면
		{
			auto PlayerController = Cast<ABAPlayerController>(EventInstigator);		// 플레이어컨트롤러를 가져와서 경험치적용 함수 호출, 이름이 ABAPlayerController 일경우 같은 이름 사용으로 오류 발생 
			BACHECK(nullptr != PlayerController, 0.0f);
			PlayerController->NPCKill(this);
		}
	}

	// 죽는 경우는 OnHPIsZero 델리게이트와 바인딩됨
	//if (FinalDamage > 0.0f)
	//{
	//	BAAnim->SetDeadAnim();
	//	SetActorEnableCollision(false);		// 죽으면 충돌 없게
	//	// 래그돌 테스트
	//	//GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	//	//GetMesh()->SetSimulatePhysics(true);		
	//}


	return FinalDamage;
}

// 사용하지않음
void ABACharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// ECharacterState::READY 상태로 옮김
	//if (IsPlayerControlled())
	//{
	//	SetControlMode(EControlMode::DIABLO);
	//	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	//}
	//else
	//{
	//	SetControlMode(EControlMode::NPC);
	//	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	//}
}

bool ABACharacter::CanSetWeapon()
{
	//return 	(nullptr == CurrentWeapon);			// 무기가 없으면 무기 장착
	return 	true;			// 항상 무기가 바뀜
}

void ABACharacter::SetWeapon(ABAWeapon* NewWeapon)
{
	BACHECK(nullptr != NewWeapon);

	if (nullptr != CurrentWeapon)
	{
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);		// 기존에 부착된 무기 액터 분리
		CurrentWeapon->Destroy();
		CurrentWeapon = nullptr;
	}

	//BACHECK(nullptr != NewWeapon && nullptr == CurrentWeapon);					// CanSetWeapon() 이 항상 true 이므로 주석처리

	// 무기액터 스폰시켜 오른손에 붙이기, 액터스폰은 BeginPlay 시점부터 사용가능
	FName WeaponSocket(TEXT("hand_rSocket"));
	//auto CurWeapon = GetWorld()->SpawnActor<ABAWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);

	if (nullptr != NewWeapon)
	{
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

float ABACharacter::GetFinalAttackRange() const
{
	return (nullptr != CurrentWeapon) ? CurrentWeapon->GetAttackRange() : AttackRange;
}

float ABACharacter::GetFinalAttackDamage() const
{
	float AttackDamage = (nullptr != CurrentWeapon) ? (CharacterStat->GetAttack() + CurrentWeapon->GetAttackDamage()) : (CharacterStat->GetAttack());
	float AttackModifier = (nullptr != CurrentWeapon) ? (CurrentWeapon->GetAttackModifier()) : (1.0f);

	return AttackDamage * AttackModifier;
}

void ABACharacter::OnAssetLoadCompleted()
{
	USkeletalMesh* AssetLoaded = Cast<USkeletalMesh>(AssetStreamingHandle->GetLoadedAsset());
	AssetStreamingHandle.Reset();
	BACHECK(nullptr != AssetLoaded);
	GetMesh()->SetSkeletalMesh(AssetLoaded);

	// 에셋 로딩이 끝나면 레디 상태
	SetCharacterState(ECharacterState::READY);

	// 이렇게해도 동작함
	/*
	AssetStreamingHandle->ReleaseHandle();
	TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);
	if (LoadedAssetPath.IsValid())
	{
		GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
	}
	*/
}