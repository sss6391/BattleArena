// Fill out your copyright notice in the Description page of Project Settings.


#include "BAAIController.h"
#include <AIModule/Classes/BehaviorTree/BehaviorTree.h>
#include <AIModule/Classes/BehaviorTree/BlackboardData.h>
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

// BT사용없이 움직임 테스트용
/*
#include "NavigationSystem.h"						// 네비시스템 클래스			
#include "Blueprint/AIBlueprintHelperLibrary.h"		// SimpleMoveToLocation 함수 사용
*/

// 블랙보드에 추가된 키 이름
const FName ABAAIController::HomePosKey(TEXT("HomePos"));
const FName ABAAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName ABAAIController::TargetKey(TEXT("Target"));

ABAAIController::ABAAIController()
{
	// 블랙보드
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObjcet(TEXT("/Game/Book/AI/BB_BACharacter.BB_BACharacter"));
	if (BBObjcet.Succeeded())
	{
		BBAsset = BBObjcet.Object;
	}

	// 트리
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
	
	// RunAI() 에서 수동으로 시작
	//if (UseBlackboard(BBAsset, Blackboard))
	//{
	//	// 블랙보드 HomePosKey , 현재위치 저장
	//	Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());

	//	if (!RunBehaviorTree(BTAsset))
	//	{
	//		BALOG(Error, TEXT("AIController Couldn't run behavior tree!!!!"));
	//	}
	//}

	// 3초 타이머 함수 설정, BT사용없이 움직임 테스트
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &ABAAIController::OnRepeatTimer, RepeatInterval, true);	
}

void ABAAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// BT사용없이 움직임 테스트
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void ABAAIController::RunAI()
{
	if (UseBlackboard(BBAsset, Blackboard))
	{
		// 블랙보드 HomePosKey , 현재위치 저장
		Blackboard->SetValueAsVector(HomePosKey, GetPawn()->GetActorLocation());

		if (!RunBehaviorTree(BTAsset))
		{
			BALOG(Error, TEXT("AIController Couldn't run behavior tree!!!!"));
		}
	}
}

void ABAAIController::StopAI()
{
	// BT 정지
	auto BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BehaviorTreeComponent)
	{
		BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
	}
}

// BT사용없이 움직임 테스트
/*
void ABAAIController::OnRepeatTimer()
{
	auto CurrentPawn = GetPawn();
	BACHECK(nullptr != CurrentPawn);
	
	// 월드에 배치된 네비시스템을 가져옴
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	if (nullptr == NavSystem) return;

	BALOG_S(Warning);

	// 랜덤반경 위치로 이동
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
