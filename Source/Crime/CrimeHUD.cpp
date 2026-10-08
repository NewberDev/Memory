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

	BaseWidgetInstance->AddToViewport();
	//HideBaseWidget();
	SetInteracterKeyVisible(false);
}

void ACrimeHUD::ShowBaseWidget()
{
	if (IsValid(BaseWidgetInstance))
	{
		BaseWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACrimeHUD::HideBaseWidget()
{
	if (IsValid(BaseWidgetInstance))
	{
		BaseWidgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	}
}

