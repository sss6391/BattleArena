// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BAAIController.h"                                         // 컨트롤러 헤드 추가
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "NavigationSystem.h"						                // 네비시스템 클래스			


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
    NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    // 폰가져오기
    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if(nullptr == ControllingPawn)      return EBTNodeResult::Failed;


    // 네비시스템 가져오기
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (nullptr == NavSystem)      return EBTNodeResult::Failed;


    // 처음 스폰 위치 가져오기
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ABAAIController::HomePosKey);


    // 처음 스폰 기준 500반경 랜덤 위치로 PatrolPosKey 쓰기
    FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABAAIController::PatrolPosKey, NextPatrol.Location);
        return EBTNodeResult::Succeeded;
	}

    return EBTNodeResult::Failed;
}
