// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_Detect.h"
#include "BAAIController.h"
#include "BACharacter.h"
#include <AIModule/Classes/BehaviorTree/BlackboardComponent.h>
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)      return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;								// �ݰ� 6����

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	// �߽��� �������� �ٸ� NPC�� �����Ͽ� ������
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto OverlapResult : OverlapResults)
		{
			ABACharacter* BACharacter = Cast<ABACharacter>(OverlapResult.GetActor());
			
			// �÷��̾ �������� ĳ���͸�
			if (BACharacter && BACharacter->GetController()->IsPlayerController())
			{
				// ������ Ű�� ����
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABAAIController::TargetKey, BACharacter);

				// �����ϸ� �׸����� ǥ��
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// ������ ĳ���ͱ��� ������ ����
				DrawDebugPoint(World, BACharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), BACharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABAAIController::TargetKey, nullptr);
	}


	// ����׿� ��ο�
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
