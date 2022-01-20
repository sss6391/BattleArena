// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include <Engine/Classes/Components/BoxComponent.h>				// UBoxComponent 박스 콜리전 컴포넌트
#include "GameFramework/Actor.h"
#include "BASection.generated.h"

UCLASS()
class BATTLEARENA_API ABASection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABASection();
	
	// 작업중에도 미리 결과 확인이 가능하다
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


// 전투 스테이트
private:
	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	ESectionState CurrentState = ESectionState::READY;
	void SetState(ESectionState NewState);
	void OperateGates(bool bOpen = true);

private:
	// 맵의 기본 메쉬
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrtivateAccess = true))
	UStaticMeshComponent* Mesh;

	// 같은 여러개의 게이트 메쉬를 묶어서 관리
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrtivateAccess = true))
	TArray<UStaticMeshComponent*> GateMeshes;

	// 박스 트리거
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrtivateAccess = true))
	TArray<UBoxComponent*> GateTriggers;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrtivateAccess = true))
	UBoxComponent* Trigger;

	// 전투 초기화 상태
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrtivateAccess = true))
	bool bNoBattle;

	// 중앙 트리거
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 게이트 감지 트리거
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnNPCSpawn();

	UFUNCTION()
	void OnKeyNPCDestroyed(AActor* DestoryedActor);

	// 스폰 타임
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float EnemySpawnTime;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float ItemBoxSpawnTime;

	// 타이머 핸들
	FTimerHandle SpawnNPCTimerHandle = { };
	FTimerHandle SpawnItemBoxTimerHandle = { };


};
