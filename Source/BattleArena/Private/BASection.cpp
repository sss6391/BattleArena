// Fill out your copyright notice in the Description page of Project Settings.
// BAtrigger 프리셋을 만들어서 BACharacter만 콜리전 되도록 설정
// 무한 맵생성, 맵 상태 ,맵생성시 아이템과 NPC생성, NPC죽으면 점수 올리도록 

#include "BASection.h"
#include "BACharacter.h"
#include "BAItemBox.h"
#include "BAPlayerController.h"
#include "BAGameMode.h"


// Sets default values
ABASection::ABASection()
{
	PrimaryActorTick.bCanEverTick = false;		// 틱은 사용하지 않음

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
    RootComponent = Mesh;

    // 광장 메쉬, SM_SQUARE 메쉬 고정적으로 사용함
    FString AssetPath = TEXT("/Game/Book/StaticMesh/SM_SQUARE.SM_SQUARE");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SQUARE(*AssetPath);
    if (SM_SQUARE.Succeeded())
    {
        Mesh->SetStaticMesh(SM_SQUARE.Object);
    }
    else
    {
        BALOG(Error, TEXT("Failed to load staticMesh : %s"), *AssetPath);
    }

    // 중앙 큰 트리거 박스 생성
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
    Trigger->SetCollisionProfileName(TEXT("BATrigger"));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABASection::OnTriggerBeginOverlap);          // 트리거 델리게이트연결

    // 게이트 메쉬
    FString GateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
    if (!SM_GATE.Succeeded())
    {
        BALOG(Error, TEXT("Failed to load staticMesh : %s"), *GateAssetPath);
    }

    // 게이트가 부착될 광장 메쉬 소켓이름
    static FName GateSockets[] = {
        {TEXT("+XGate")},
        {TEXT("-XGate")},
        {TEXT("+YGate")},
        {TEXT("-YGate")}
    };

    // 게이트 메쉬와 게이트 트리거 생성후 소켓에 붙이기
    for (FName GateSocket : GateSockets)
    {
        // 게이트 메쉬
        BACHECK(Mesh->DoesSocketExist(GateSocket));
        UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
        NewGate->SetStaticMesh(SM_GATE.Object);
        NewGate->SetupAttachment(RootComponent, GateSocket);
        NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));      // 위치조절
        GateMeshes.Add(NewGate);

        // 게이트용 트리거 박스들
        UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
        NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
        NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
        NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
        NewGateTrigger->SetCollisionProfileName(TEXT("BATrigger"));
        GateTriggers.Add(NewGateTrigger);

        NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABASection::OnGateTriggerBeginOverlap);       // 게이트 트리거 델리게이트 연결
        NewGateTrigger->ComponentTags.Add(GateSocket);                                                          // 게이트를 구분하기 위한 태그
    }

    bNoBattle = false;

    // 스폰시간 지정
    EnemySpawnTime = 2.0f;
    ItemBoxSpawnTime = 5.0f;
}

// 플레이하지않아도 변수편집으로 인해 함수가 실행되어서 바뀐 모습을 볼수 있다.
void ABASection::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
  //  SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::BATTLE);
}

// Called when the game starts or when spawned
void ABASection::BeginPlay()
{
	Super::BeginPlay();

    SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void ABASection::SetState(ESectionState NewState)
{
    switch (NewState)
    {
    case ESectionState::READY:
        Trigger->SetCollisionProfileName(TEXT("BATrigger"));
        for (UBoxComponent* GateTrigger : GateTriggers)
        {
            GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
        }
        OperateGates(true);

        break;

    case ESectionState::BATTLE:
        Trigger->SetCollisionProfileName(TEXT("NoCollision"));
        for (UBoxComponent* GateTrigger : GateTriggers)
        {
            GateTrigger->SetCollisionProfileName(TEXT("NoCollision"));
        }
        OperateGates(false);

        // 적 스폰 타이머 설정 , 스폰시 네비 메쉬가 설정이 안되어서 움직이지 않을 수 있다., 동적 네비게이션메쉬를 생성한다, 프로젝트설정- 네비게이션 메시-Runtime Generation- Dynamic
        GetWorld()->GetTimerManager().SetTimer(
            SpawnNPCTimerHandle,
            FTimerDelegate::CreateUObject(this, &ABASection::OnNPCSpawn),
            EnemySpawnTime, false);

        // 아이템 박스 스폰 타이머 설정
        GetWorld()->GetTimerManager().SetTimer(
            SpawnItemBoxTimerHandle,
            FTimerDelegate::CreateLambda([this]()-> void
                {
                    FVector2D RandXY = FMath::RandPointInCircle(600.0f);
                    GetWorld()->SpawnActor<ABAItemBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
                }), ItemBoxSpawnTime, false);

        break;

    case ESectionState::COMPLETE:
        Trigger->SetCollisionProfileName(TEXT("NoCollision"));
        for (UBoxComponent* GateTrigger : GateTriggers)
        {
            GateTrigger->SetCollisionProfileName(TEXT("BATrigger"));
        }
        OperateGates(true);

        break;

    default:
        break;
    }
}

void ABASection::OperateGates(bool bOpen)
{
    for (UStaticMeshComponent* Gate : GateMeshes)
    {
        Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);            // ture면 문이 열린 상태
    }
}

void ABASection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 새로 생성되면 READY상태, 중앙트리거에 들어오면 배틀상태로
    if (CurrentState == ESectionState::READY)
    {
        SetState(ESectionState::BATTLE);
        
        // 무한맵 테스트용
        //SetState(ESectionState::COMPLETE);
    }
}

// 게이트 트리거에 닿으면 해당 위치에 맵생성
void ABASection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    BACHECK(OverlappedComponent->ComponentTags.Num() == 1);
    FName ComponentTag = OverlappedComponent->ComponentTags[0];
    FName SocketName = FName(*ComponentTag.ToString().Left(2));
    
    // 이미 맵이 존재하면 생성 안하기
    if (!Mesh->DoesSocketExist(SocketName))
        return;


    FVector NewLocation = Mesh->GetSocketLocation(SocketName);

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
    FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
    
    // 해당 위치에 맵이 있는지 콜리전으로 검사
    bool bResult = GetWorld()->OverlapMultiByObjectType(
        OverlapResults,
        NewLocation,
        FQuat::Identity,
        ObjectQueryParam,
        FCollisionShape::MakeSphere(775.0f),
        CollisionQueryParam
    );

    if (!bResult)
    {
        auto NewSection = GetWorld()->SpawnActor<ABASection>(NewLocation, FRotator::ZeroRotator);
    }
    else
    {
        BALOG(Warning, TEXT("New section area is not empty."));
    }
}

void ABASection::OnNPCSpawn()
{
    GetWorld()->GetTimerManager().ClearTimer(SpawnNPCTimerHandle);
    auto KeyNPC = GetWorld()->SpawnActor<ABACharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);      // NPC 액터생성
    if (nullptr != KeyNPC)
    {
        KeyNPC->OnDestroyed.AddDynamic(this, &ABASection::OnKeyNPCDestroyed);               // 생성된 액터가 파괴되면 알리는 OnDestroyed 에 바인딩
    }
}

// 액터가 파괴되면 호출
void ABASection::OnKeyNPCDestroyed(AActor* DestoryedActor)
{
    auto BACharacter = Cast<ABACharacter>(DestoryedActor);
    BACHECK(nullptr != BACharacter);

    auto BAPlayerController = Cast<ABAPlayerController>(BACharacter->LastHitBy);            // 마지막으로 때린 컨트롤러 캐스팅하여 확인
    BACHECK(nullptr != BAPlayerController);

    // 게임모드에서 점수를 올린다
    auto ABGameMode = Cast<ABAGameMode>(GetWorld()->GetAuthGameMode());
    BACHECK(nullptr != ABGameMode);
    ABGameMode->AddScore(BAPlayerController);

    SetState(ESectionState::COMPLETE);
}
