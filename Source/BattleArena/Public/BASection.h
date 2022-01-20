// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include <Engine/Classes/Components/BoxComponent.h>				// UBoxComponent �ڽ� �ݸ��� ������Ʈ
#include "GameFramework/Actor.h"
#include "BASection.generated.h"

UCLASS()
class BATTLEARENA_API ABASection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABASection();
	
	// �۾��߿��� �̸� ��� Ȯ���� �����ϴ�
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


// ���� ������Ʈ
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
	// ���� �⺻ �޽�
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrtivateAccess = true))
	UStaticMeshComponent* Mesh;

	// ���� �������� ����Ʈ �޽��� ��� ����
	UPROPERTY(VisibleAnywhere, Category = Mesh, Meta = (AllowPrtivateAccess = true))
	TArray<UStaticMeshComponent*> GateMeshes;

	// �ڽ� Ʈ����
	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrtivateAccess = true))
	TArray<UBoxComponent*> GateTriggers;

	UPROPERTY(VisibleAnywhere, Category = Trigger, Meta = (AllowPrtivateAccess = true))
	UBoxComponent* Trigger;

	// ���� �ʱ�ȭ ����
	UPROPERTY(EditAnywhere, Category = State, Meta = (AllowPrtivateAccess = true))
	bool bNoBattle;

	// �߾� Ʈ����
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ����Ʈ ���� Ʈ����
	UFUNCTION()
	void OnGateTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnNPCSpawn();

	UFUNCTION()
	void OnKeyNPCDestroyed(AActor* DestoryedActor);

	// ���� Ÿ��
	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float EnemySpawnTime;

	UPROPERTY(EditAnywhere, Category = Spawn, Meta = (AllowPrivateAccess = true))
	float ItemBoxSpawnTime;

	// Ÿ�̸� �ڵ�
	FTimerHandle SpawnNPCTimerHandle = { };
	FTimerHandle SpawnItemBoxTimerHandle = { };


};
