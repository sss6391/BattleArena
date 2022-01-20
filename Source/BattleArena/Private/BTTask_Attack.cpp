// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BAAIController.h"
#include "BACharacter.h"


UBTTask_Attack::UBTTask_Attack()
{
    bNotifyTick = true; //틱 기능 활성화
    IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto BACharacter = Cast<ABACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == BACharacter)         return EBTNodeResult::Failed;

    BACharacter->Attack();
    IsAttacking = true;

    BACharacter->OnAttackEnd.AddLambda([this]() ->void {IsAttacking = false; });    // 캐릭터의 모션이 끝나는 시점을 브로드캐스트하는 델리게이트로 연결시켜놓음

    return EBTNodeResult::InProgress;                                           // 테스크가 진행중임을 알려서 다음으로 넘어가지 않도록
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // 틱으로 돌다가 테스크가 완료되면 알려줌
    if (!IsAttacking)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);                      
    }
}