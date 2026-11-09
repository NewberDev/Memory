// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimeHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"

void ACrimeHUD::BeginPlay()
{
	Super::BeginPlay();

	CreateBaseWidget();

	HideBaseWidget();
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

void ACrimeHUD::OpenItemInspectWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass || IsValid(ItemInspectWidgetInstance)) return;

	APlayerController* PlayerController = GetOwningPlayerController();

	if (!IsValid(PlayerController)) return;

	ItemInspectWidgetInstance = CreateWidget<UUserWidget>(
		PlayerController,
		WidgetClass);

	if (!IsValid(ItemInspectWidgetInstance)) return;

	ItemInspectWidgetInstance->AddToViewport(200);

}

void ACrimeHUD::CloseItemInspectWidget()
{
	if (!IsValid(ItemInspectWidgetInstance)) return;

	ItemInspectWidgetInstance->RemoveFromParent();
	ItemInspectWidgetInstance = nullptr;
}

void ACrimeHUD::OpenEscWidget()
{
	if (!EscWidgetClass) return;

	if (!IsValid(EscWidgetInstance))
	{
		APlayerController* PlayerController = GetOwningPlayerController();

		if (!IsValid(PlayerController)) return;

		EscWidgetInstance = CreateWidget<UUserWidget>(
			PlayerController, EscWidgetClass);
	}

	if (!IsValid(EscWidgetInstance)) return;

	if (!EscWidgetInstance->IsInViewport())
	{
		EscWidgetInstance->AddToViewport(300);
	}

	// WBP_Esc가 Esc 키를 받을 수 있도록 포커스 지정
	EscWidgetInstance->SetKeyboardFocus();
}

void ACrimeHUD::CloseEscWidget()
{
	if (!IsValid(EscWidgetInstance)) return;

	EscWidgetInstance->RemoveFromParent();

}