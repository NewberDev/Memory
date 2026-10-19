// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemDatabaseAsset.h"
#include "CrimeGameInstanceSubsystem.generated.h"

class UInventoryComponent;
class UUserWidget;

/**
 * 
 */
UCLASS()
class CRIME_API UCrimeGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	bool bInteracterKeyRequestedVisible = false;

public:

	// 현재 월드의 GameInstanceSubsystem을 가져온다.
	UFUNCTION(BlueprintPure, Category = "Subsystem", meta = (WorldContext = "WorldContextObject"))
	static UCrimeGameInstanceSubsystem* Get(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	UInventoryComponent* GetPlayerInventory() const;

	// 현재 플레이어 HUD의 E 키 표시 함수를 호출한다.
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void SetInteracterKeyVisible(bool bVisible);

	UFUNCTION(BlueprintCallable, Category = "Puzzle")
	void SetPuzzleSceneActive(bool bActive, UUserWidget* PuzzleWidget);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItemToPlayerInventory(
		//UPARAM(meta = (GetOptions = "Crime.CrimeGameInstanceSubsystem.GetItemIdOptions"))
		FName ItemId
	);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool CheckPlayerInventorySpace();

	UFUNCTION(BlueprintPure, Category = "Item Data", meta = (DisplayName = "Select Item ID"))
	FName SelectItemId(
		UPARAM(meta = (GetOptions ="Crime.CrimeGameInstanceSubsystem.GetItemIdOptions"))
		FName ItemId
	) const;

	//ItemDataAsset의 항목을 반환하기 위한 함수. 
	UFUNCTION()
	static TArray<FString> GetItemIdOptions();

	UFUNCTION(BlueprintPure, Category = "Text Data",
		meta = (DisplayName = "Select Text ID"))
	FName SelectTextId(
		UPARAM(meta = (GetOptions = "Crime.CrimeGameInstanceSubsystem.GetTextIdOptions"))
		FName TextId
	) const;

	// TextDataAsset의 항목을 반환하기 위한 함수.
	UFUNCTION()
	static TArray<FString> GetTextIdOptions();

	UFUNCTION(BlueprintCallable, Category = "HUD|Messages")
	void ShowCenterMessageText(const FString& Message);

	UFUNCTION(BlueprintCallable, Category = "HUD|Messages")
	void ShowCenterMessageTextId(
		UPARAM(meta = (GetOptions = "Crime.CrimeGameInstanceSubsystem.GetTextIdOptions"))
		FName TextId
	);

	UFUNCTION(BlueprintCallable, Category = "HUDFade")
	void OnFadeEffect();

	UFUNCTION(BlueprintCallable, Category = "HUD|Inventory")
	void PlayInventoryHintAnimation(int32 InventoryIndex);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetPlayerControlEnabled(bool bEnabled);

	void RefreshInteracterKeyVisibility();

	UFUNCTION(BlueprintCallable, Category = "HUD")
	void HidePlayerBaseUI();
	
};
