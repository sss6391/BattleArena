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
	float DetectRadius = 600.0f;								// 반경 6미터

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);

	// 중심을 기준으로 다른 NPC들 포함하여 감지함
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
			
			// 플레이어가 조종중인 캐릭터만
			if (BACharacter && BACharacter->GetController()->IsPlayerController())
			{
				// 블랙보드 키에 저장
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(ABAAIController::TargetKey, BACharacter);

				// 감지하면 그린으로 표시
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);

				// 감지된 캐릭터까지 선으로 연결
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


	// 디버그용 드로우
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
}
