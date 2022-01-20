// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "BAAIController.h"
#include "BACharacter.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
    NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
    bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

   // auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    auto ControllingPawn = Cast<ABACharacter>(OwnerComp.GetAIOwner()->GetPawn());       
    if (nullptr == ControllingPawn)       return false;

    auto Target = Cast<ABACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABAAIController::TargetKey));
    if (nullptr == Target)     return false;

  
    //bResult = (Target->GetDistanceTo(ControllingPawn) <= 200.0f);                                 // ������ �Ÿ��� 2���� ���ϸ� ��
    bResult = (Target->GetDistanceTo(ControllingPawn) <= ControllingPawn->GetFinalAttackRange());   // ������ ���� ���� ���� ���ϸ� �� (����=150, �Ǽ�=80 ��������)

    // �׽�Ʈ��
    //FString sResult = bResult ? TEXT("True") : TEXT("Flase");
    //BALOG(Warning, TEXT("Distance %f , result = %s"), Target->GetDistanceTo(ControllingPawn), *sResult);

    return bResult;
}
