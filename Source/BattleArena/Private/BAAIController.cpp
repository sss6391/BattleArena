// Fill out your copyright notice in the Description page of Project Settings.


#include "BAAIController.h"
#include <AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <AIModule/Classes/BehaviorTree/BlackboardData.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

// BT������ ������ �׽�Ʈ��
/*
#include "NavigationSystem.h"						// �׺�ý��� Ŭ����			
#include "Blueprint/AIBlueprintHelperLibrary.h"		// SimpleMoveToLocation �Լ� ���
*/

// �����忡 �߰��� Ű �̸�
const FName ABAAIController::HomePosKey(TEXT("HomePos"));
const FName ABAAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABAAIController::TargetKey(TEXT("Target"));

ABAAIController::ABAAIController()
{
	// ������
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjcet(TEXT("/Game/Book/AI/BB_BACharacter.BB_BACharacter"));
	if (BBObjcet.Succeeded())
	{
		BBAsset = BBObjcet.Object;
	}

	// Ʈ��
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObjcet(TEXT("/Game/Book/AI/BT_BACharacter.BT_BACharacter"));
	if (BTObjcet.Succeeded())
	{
		BTAsset = BTObjcet.Object;
	}


	//RepeatInterval = 3.0f;
}

void ABAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// RunAI() ���� �������� ����
	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	// ������ HomePosKey , ������ġ ����
	//	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		BALOG(Error, TEXT("AIController Couldn't run behavior tree!!!!"));
	//	}
	//}

	// 3�� Ÿ�̸� �Լ� ����, BT������ ������ �׽�Ʈ
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ABAAIController::OnRepeatTimer, RepeatInterval, true);	
}

void ABAAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// BT������ ������ �׽�Ʈ
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ABAAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// ������ HomePosKey , ������ġ ����
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			BALOG(Error, TEXT("AIController Couldn't run behavior tree!!!!"));
		}
	}
}

void ABAAIController::StopAI()
{
	// BT ����
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

// BT������ ������ �׽�Ʈ
/*
void ABAAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	BACHECK(nullptr != CurrentPawn);
	
	// ���忡 ��ġ�� �׺�ý����� ������
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (nullptr == NavSystem) return;

	BALOG_S(Warning);

	// �����ݰ� ��ġ�� �̵�
	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		BALOG(Warning, TEXT("Next Location : %s"), *NextLocation.Location.ToString());
	}
	else
	{
		BALOG_S(Warning);
	}
}
*/
