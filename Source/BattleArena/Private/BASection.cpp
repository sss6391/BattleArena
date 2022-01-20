// Fill out your copyright notice in the Description page of Project Settings.
// BAtrigger �������� ���� BACharacter�� �ݸ��� �ǵ��� ����
// ���� �ʻ���, �� ���� ,�ʻ����� �����۰� NPC����, NPC������ ���� �ø����� 

#include "BASection.h"
#include "BACharacter.h"
#include "BAItemBox.h"
#include "BAPlayerController.h"
#include "BAGameMode.h"


// Sets default values
ABASection::ABASection()
{
	PrimaryActorTick.bCanEverTick = false;		// ƽ�� ������� ����

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
    RootComponent = Mesh;

    // ���� �޽�, SM_SQUARE �޽� ���������� �����
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

    // �߾� ū Ʈ���� �ڽ� ����
    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
    Trigger->SetBoxExtent(FVector(775.0f, 775.0f, 300.0f));
    Trigger->SetupAttachment(RootComponent);
    Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
    Trigger->SetCollisionProfileName(TEXT("BATrigger"));
    Trigger->OnComponentBeginOverlap.AddDynamic(this, &ABASection::OnTriggerBeginOverlap);          // Ʈ���� ��������Ʈ����

    // ����Ʈ �޽�
    FString GateAssetPath = TEXT("/Game/Book/StaticMesh/SM_GATE.SM_GATE");
    static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_GATE(*GateAssetPath);
    if (!SM_GATE.Succeeded())
    {
        BALOG(Error, TEXT("Failed to load staticMesh : %s"), *GateAssetPath);
    }

    // ����Ʈ�� ������ ���� �޽� �����̸�
    static FName GateSockets[] = {
        {TEXT("+XGate")},
        {TEXT("-XGate")},
        {TEXT("+YGate")},
        {TEXT("-YGate")}
    };

    // ����Ʈ �޽��� ����Ʈ Ʈ���� ������ ���Ͽ� ���̱�
    for (FName GateSocket : GateSockets)
    {
        // ����Ʈ �޽�
        BACHECK(Mesh->DoesSocketExist(GateSocket));
        UStaticMeshComponent* NewGate = CreateDefaultSubobject<UStaticMeshComponent>(*GateSocket.ToString());
        NewGate->SetStaticMesh(SM_GATE.Object);
        NewGate->SetupAttachment(RootComponent, GateSocket);
        NewGate->SetRelativeLocation(FVector(0.0f, -80.5f, 0.0f));      // ��ġ����
        GateMeshes.Add(NewGate);

        // ����Ʈ�� Ʈ���� �ڽ���
        UBoxComponent* NewGateTrigger = CreateDefaultSubobject<UBoxComponent>(*GateSocket.ToString().Append(TEXT("Trigger")));
        NewGateTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 300.0f));
        NewGateTrigger->SetupAttachment(RootComponent, GateSocket);
        NewGateTrigger->SetRelativeLocation(FVector(70.0f, 0.0f, 250.0f));
        NewGateTrigger->SetCollisionProfileName(TEXT("BATrigger"));
        GateTriggers.Add(NewGateTrigger);

        NewGateTrigger->OnComponentBeginOverlap.AddDynamic(this, &ABASection::OnGateTriggerBeginOverlap);       // ����Ʈ Ʈ���� ��������Ʈ ����
        NewGateTrigger->ComponentTags.Add(GateSocket);                                                          // ����Ʈ�� �����ϱ� ���� �±�
    }

    bNoBattle = false;

    // �����ð� ����
    EnemySpawnTime = 2.0f;
    ItemBoxSpawnTime = 5.0f;
}

// �÷��������ʾƵ� ������������ ���� �Լ��� ����Ǿ �ٲ� ����� ���� �ִ�.
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

        // �� ���� Ÿ�̸� ���� , ������ �׺� �޽��� ������ �ȵǾ �������� ���� �� �ִ�., ���� �׺���̼Ǹ޽��� �����Ѵ�, ������Ʈ����- �׺���̼� �޽�-Runtime Generation- Dynamic
        GetWorld()->GetTimerManager().SetTimer(
            SpawnNPCTimerHandle,
            FTimerDelegate::CreateUObject(this, &ABASection::OnNPCSpawn),
            EnemySpawnTime, false);

        // ������ �ڽ� ���� Ÿ�̸� ����
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
        Gate->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);            // ture�� ���� ���� ����
    }
}

void ABASection::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // ���� �����Ǹ� READY����, �߾�Ʈ���ſ� ������ ��Ʋ���·�
    if (CurrentState == ESectionState::READY)
    {
        SetState(ESectionState::BATTLE);
        
        // ���Ѹ� �׽�Ʈ��
        //SetState(ESectionState::COMPLETE);
    }
}

// ����Ʈ Ʈ���ſ� ������ �ش� ��ġ�� �ʻ���
void ABASection::OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    BACHECK(OverlappedComponent->ComponentTags.Num() == 1);
    FName ComponentTag = OverlappedComponent->ComponentTags[0];
    FName SocketName = FName(*ComponentTag.ToString().Left(2));
    
    // �̹� ���� �����ϸ� ���� ���ϱ�
    if (!Mesh->DoesSocketExist(SocketName))
        return;


    FVector NewLocation = Mesh->GetSocketLocation(SocketName);

    TArray<FOverlapResult> OverlapResults;
    FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
    FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
    
    // �ش� ��ġ�� ���� �ִ��� �ݸ������� �˻�
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
    auto KeyNPC = GetWorld()->SpawnActor<ABACharacter>(GetActorLocation() + FVector::UpVector * 88.0f, FRotator::ZeroRotator);      // NPC ���ͻ���
    if (nullptr != KeyNPC)
    {
        KeyNPC->OnDestroyed.AddDynamic(this, &ABASection::OnKeyNPCDestroyed);               // ������ ���Ͱ� �ı��Ǹ� �˸��� OnDestroyed �� ���ε�
    }
}

// ���Ͱ� �ı��Ǹ� ȣ��
void ABASection::OnKeyNPCDestroyed(AActor* DestoryedActor)
{
    auto BACharacter = Cast<ABACharacter>(DestoryedActor);
    BACHECK(nullptr != BACharacter);

    auto BAPlayerController = Cast<ABAPlayerController>(BACharacter->LastHitBy);            // ���������� ���� ��Ʈ�ѷ� ĳ�����Ͽ� Ȯ��
    BACHECK(nullptr != BAPlayerController);

    // ���Ӹ�忡�� ������ �ø���
    auto ABGameMode = Cast<ABAGameMode>(GetWorld()->GetAuthGameMode());
    BACHECK(nullptr != ABGameMode);
    ABGameMode->AddScore(BAPlayerController);

    SetState(ESectionState::COMPLETE);
}
