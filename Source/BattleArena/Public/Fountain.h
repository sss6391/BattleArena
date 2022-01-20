// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//UStaticMeshComponent ������
//#include <Engine/Classes/Components/StaticMeshComponent.h>
#include <Engine/Classes/Components/PointLightComponent.h>
#include <Engine/Classes/Particles/ParticleSystemComponent.h>
#include <Engine/Classes/GameFramework/RotatingMovementComponent.h>
//#include "GameFramework/RotatingMovementComponent.h"
// �α� ī�װ� �߰���
#include "BattleArena.h"
#include "Fountain.generated.h"

UCLASS()
class BATTLEARENA_API AFountain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFountain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Water;

	UPROPERTY(VisibleAnywhere)
	UPointLightComponent* Light;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Splash;

	UPROPERTY(EditAnywhere, Category=ID)
	int32 ID;

	UPROPERTY(VisibleAnywhere)
	URotatingMovementComponent* Movement;

private:

	// AllowPrivateAccess ĸ��ȭ
	UPROPERTY(EditAnywhere, Category=Stat, Meta = (AllowPrivateAccess = true))
	float RotateSpeed;

};
