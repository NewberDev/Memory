// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimeHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void ACrimeHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateBaseWidget();
}

void ACrimeHUD::CreateBaseWidget()
{
	if (IsValid(BaseWidgetInstance))
	{
		return;
	}

	APlayerController* PlayerController = GetOwningPlayerController();
	if (!PlayerController || !BaseWidgetClass)
	{
		return;
	}

	BaseWidgetInstance = CreateWidget<UUserWidget>(
		PlayerController,
		BaseWidgetClass
	);

	if (!BaseWidgetInstance)
	{
		return;
	}

	BaseWidgetInstance->AddToViewport(100);
	
	OnBaseWidgetCreated(BaseWidgetInstance);

	SetInteracterKeyVisible(false);
}

