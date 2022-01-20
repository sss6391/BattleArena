// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "BAAIController.h"
#include "BACharacter.h"


UBTTask_Attack::UBTTask_Attack()
{
    bNotifyTick = true; //ƽ ��� Ȱ��ȭ
    IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto BACharacter = Cast<ABACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == BACharacter)         return EBTNodeResult::Failed;

    BACharacter->Attack();
    IsAttacking = true;

    BACharacter->OnAttackEnd.AddLambda([this]() ->void {IsAttacking = false; });    // ĳ������ ����� ������ ������ ��ε�ĳ��Ʈ�ϴ� ��������Ʈ�� ������ѳ���

    return EBTNodeResult::InProgress;                                           // �׽�ũ�� ���������� �˷��� �������� �Ѿ�� �ʵ���
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    // ƽ���� ���ٰ� �׽�ũ�� �Ϸ�Ǹ� �˷���
    if (!IsAttacking)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);                      
    }
}