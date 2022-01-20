// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "BAAIController.h"
#include "BACharacter.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
    NodeName = TEXT("Turn");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    auto ABCharacter = Cast<ABACharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == ABCharacter)         return EBTNodeResult::Failed;

    auto Target = Cast<ABACharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ABAAIController::TargetKey));
    if (nullptr == Target)              return EBTNodeResult::Failed;

    FVector LookVector = Target->GetActorLocation() - ABCharacter->GetActorLocation();
    LookVector.Z = 0.0f;

    FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();

    //  ������ �ӵ��� ȸ���ϵ��� FMath::RInterpTo �Լ��� ���
    //ABCharacter->SetActorRotation(FMath::RInterpTo(
    //    ABCharacter->GetActorRotation(),
    //    TargetRot, GetWorld()->GetDeltaSeconds(),
    //    2.0f));

    // 2������� ȸ���ϵ���
    ABCharacter->SetActorRotation(FMath::RInterpTo(
        ABCharacter->GetActorRotation(),
        TargetRot, GetWorld()->GetDeltaSeconds(),
        4.0f));

    return EBTNodeResult::Succeeded;
}
