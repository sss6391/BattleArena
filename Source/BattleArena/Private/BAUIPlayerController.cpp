// Fill out your copyright notice in the Description page of Project Settings.


#include "BAUIPlayerController.h"
#include "Blueprint/UserWidget.h"		// UUserWidget


ABAUIPlayerController::ABAUIPlayerController() 
{
	static ConstructorHelpers::FClassFinder<UUserWidget> WB_Title(TEXT("WidgetBlueprint'/Game/Book/UI/UI_Title.UI_Title_C'"));
	if (WB_Title.Succeeded())
	{
		UIWidgetClass = WB_Title.Class;
	}
}


void ABAUIPlayerController::BeginPlay()
{
	BACHECK(nullptr != UIWidgetClass);

	// À§Á¬»ý¼º
	UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
	BACHECK(nullptr != UIWidgetInstance);

	UIWidgetInstance->AddToViewport();

	FInputModeUIOnly Mode;
	Mode.SetWidgetToFocus(UIWidgetInstance->GetCachedWidget());

	SetInputMode(Mode);
	bShowMouseCursor = true;
}
