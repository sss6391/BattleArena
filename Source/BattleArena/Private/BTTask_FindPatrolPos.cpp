// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "BAAIController.h"                                         // ��Ʈ�ѷ� ��� �߰�
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "NavigationSystem.h"						                // �׺�ý��� Ŭ����			


UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
    NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    // ����������
    auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if(nullptr == ControllingPawn)      return EBTNodeResult::Failed;


    // �׺�ý��� ��������
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(ControllingPawn->GetWorld());
    if (nullptr == NavSystem)      return EBTNodeResult::Failed;


    // ó�� ���� ��ġ ��������
    FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(ABAAIController::HomePosKey);


    // ó�� ���� ���� 500�ݰ� ���� ��ġ�� PatrolPosKey ����
    FNavLocation NextPatrol;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.0f, NextPatrol))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(ABAAIController::PatrolPosKey, NextPatrol.Location);
        return EBTNodeResult::Succeeded;
	}

    return EBTNodeResult::Failed;
}
