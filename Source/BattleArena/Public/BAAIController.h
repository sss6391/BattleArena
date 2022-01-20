// �׺���̼� ����� �߰��ؾ� �׺� ��밡���ϴ�.

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

	// ������ Ű
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


	// BT������ ������ �׽�Ʈ
	/*
	void OnRepeatTimer();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;
	*/
};
