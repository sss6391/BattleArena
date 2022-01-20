// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleArena.h"
#include <Engine/Classes/Components/BoxComponent.h>				// UBoxComponent �ڽ� �ݸ��� ������Ʈ
#include <Engine/Classes/Particles/ParticleSystemComponent.h>
#include "GameFramework/Actor.h"
#include "BAItemBox.generated.h"

UCLASS()
class BATTLEARENA_API ABAItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABAItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	UPROPERTY(VisibleAnywhere, Category = Box)
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
	UStaticMeshComponent* Box;

	UPROPERTY(EditInstanceOnly, Category = Box)
	TSubclassOf<class ABAWeapon> WeaponItemClass;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	UParticleSystemComponent* Effect;

private:
	// ������ �̺�Ʈ ��������Ʈ
	// ���� ��ũ��: DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_SixParams(FComponentBeginOverlapSignature, UPrimitiveComponent, OnComponentBeginOverlap, UPrimitiveComponent*, OverlappedComponent, AActor*, OtherActor, UPrimitiveComponent*, OtherComp, int32, OtherBodyIndex, bool, bFromSweep, const FHitResult&, SweepResult);
	UFUNCTION()				// ������ ����
	void OnCharacterOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()				
	void OnEffectFinished(class UParticleSystemComponent* PSystem);

};
