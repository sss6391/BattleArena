// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BattleArena.h"
#include "GameFramework/PlayerController.h"
#include "BAUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEARENA_API ABAUIPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	ABAUIPlayerController();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadwrite, Category = UI)
	TSubclassOf<class UUserWidget> UIWidgetClass;

	UPROPERTY()
	class UUserWidget* UIWidgetInstance;
};
