// 네비게이션 모듈을 추가해야 네비를 사용가능하다.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include "AIController.h"
#include "BAAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ABAAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	// 블랙보드 키
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();

private:

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;


	// BT사용없이 움직임 테스트
	/*
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	*/
};
