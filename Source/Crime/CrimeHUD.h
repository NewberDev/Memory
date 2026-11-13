// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrimeHUD.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class CRIME_API ACrimeHUD : public AHUD
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void CreateBaseWidget();

	UFUNCTION(BlueprintImplementableEvent, Category = "HUD")
	void OnBaseWidgetCreated(UUserWidget* CreatedWidget);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void ShowBaseWidget();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void HideBaseWidget();

	// 함수 이름만 선언합니다. HUD 블루프린트에서 구현합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void SetInteracterKeyVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD|Messages")
	void ShowCenterMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD|Messages")
	void OnFadeEffect();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD|Inventory")
	void PlayInventoryHintAnimation(int32 InventoryIndex);

	UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
	void OpenItemInspectWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
	void CloseItemInspectWidget();

	void OpenEscWidget();

	void CloseEscWidget();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void ShowControls();

	// 게임 패드 선택 UI 표시 / 숨김
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD|Inventory")
	void SetInventorySelectionVisible(bool bVisible);

	// 게임 패드 선택 UI를 해당 슬롯으로 이동
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD|Inventory")
	void SetInventorySelectionIndex(int32 ItemIndex);

	// 게임 패드 선택 UI를 닫고 선택 표시를 초기화한다.
	UFUNCTION(BlueprintImplementableEvent, Category = "HUD|Inventory")
	void CancelInventorySelectionUI();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> BaseWidgetClass;

	// HUD 블루프린트에서 이 참조로 위젯 내부 이미지에 접근합니다.
	UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> BaseWidgetInstance;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> ItemInspectWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD|Settings")
	TSubclassOf<UUserWidget> EscWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> EscWidgetInstance = nullptr;
};
