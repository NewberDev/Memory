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

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowBaseWidget();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HideBaseWidget();

	// 함수 이름만 선언합니다. HUD 블루프린트에서 구현합니다.
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD")
	void SetInteracterKeyVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD|Messages")
	void ShowCenterMessage(const FString& Message);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "HUD|Messages")
	void OnFadeEffect();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<UUserWidget> BaseWidgetClass;

	// HUD 블루프린트에서 이 참조로 위젯 내부 이미지에 접근합니다.
	UPROPERTY(Transient, BlueprintReadOnly, Category = "HUD")
	TObjectPtr<UUserWidget> BaseWidgetInstance;
};
