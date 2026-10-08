// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimeGameInstanceSubsystem.h"

#include "CrimeHUD.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "CrimeGameModeBase.h"
#include "InventoryComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "ItemDatabaseAsset.h"


UCrimeGameInstanceSubsystem* UCrimeGameInstanceSubsystem::Get(const UObject* WorldContextObject)
{
	const UWorld* World =
		WorldContextObject ? WorldContextObject->GetWorld() : nullptr;

	if (!World) return nullptr;

	UGameInstance* GameInstance = World->GetGameInstance();
	if (!GameInstance)
	{
		return nullptr;
	}

	return GameInstance->GetSubsystem<UCrimeGameInstanceSubsystem>();
}

UInventoryComponent* UCrimeGameInstanceSubsystem::GetPlayerInventory() const
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return nullptr;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController)
	{
		return nullptr;
	}

	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		return nullptr;
	}

	return UInventoryComponent::FindInventory(PlayerPawn);
}

void UCrimeGameInstanceSubsystem::SetInteracterKeyVisible(bool bVisible)
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController)
	{
		return;
	}

	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();
	if (!CrimeHUD)
	{
		return;
	}

	CrimeHUD->SetInteracterKeyVisible(bVisible);
}

void UCrimeGameInstanceSubsystem::SetPuzzleSceneActive(
	bool bActive,
	UUserWidget* PuzzleWidget)
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World) return;

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController) return;

	if (bActive)
	{
		PlayerController->SetIgnoreMoveInput(true);
		PlayerController->SetIgnoreLookInput(true);

		FInputModeUIOnly InputMode;
		if (IsValid(PuzzleWidget))
		{
			InputMode.SetWidgetToFocus(PuzzleWidget->TakeWidget());
		}

		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = true;

		if (ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>())
		{
			CrimeHUD->HideBaseWidget();
		}
	}
	else
	{
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);

		FInputModeGameOnly InputMode;
		PlayerController->SetInputMode(InputMode);
		PlayerController->bShowMouseCursor = false;

		if (ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>())
		{
			CrimeHUD->ShowBaseWidget();
		}
	}
}

bool UCrimeGameInstanceSubsystem::AddItemToPlayerInventory(FName ItemId)
{
	if (ItemId.IsNone()) return false;

	UInventoryComponent* Inventory = GetPlayerInventory();
	if (!IsValid(Inventory)) return false;


	const UWorld* World = GetWorld();
	const ACrimeGameModeBase* CrimeGameMode =
		World ? World->GetAuthGameMode<ACrimeGameModeBase>() : nullptr;

	if (!IsValid(CrimeGameMode))
	{
		UE_LOG(LogTemp, Warning, TEXT("GameMode Error"));
		return false;
	}


	const UItemDatabaseAsset* DatabaseAsset =
		CrimeGameMode->GetItemDatabaseAsset();

	if (!IsValid(DatabaseAsset)) return false;


	const FInventoryItem* ItemData = DatabaseAsset->Items.FindByPredicate(
		[ItemId](const FInventoryItem& Item)
		{
			return Item.ItemId == ItemId;
		}
	);

	if (!ItemData) return false;

	return Inventory->AddItem(*ItemData);
}


TArray<FString> UCrimeGameInstanceSubsystem::GetItemIdOptions()
{
	TArray<FString> Options;

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
			TEXT("AssetRegistry")
		);

	TArray<FAssetData> DatabaseAssets;
	AssetRegistryModule.Get().GetAssetsByClass(
		UItemDatabaseAsset::StaticClass()->GetClassPathName(),
		DatabaseAssets,
		true
	);

	for (const FAssetData& AssetData : DatabaseAssets)
	{
		const UItemDatabaseAsset* DatabaseAsset =
			Cast<UItemDatabaseAsset>(AssetData.GetAsset());

		if (!IsValid(DatabaseAsset))
		{
			continue;
		}

		for (const FInventoryItem& Item : DatabaseAsset->Items)
		{
			if (!Item.ItemId.IsNone())
			{
				Options.AddUnique(Item.ItemId.ToString());
			}
		}
	}

	return Options;
}

FName UCrimeGameInstanceSubsystem::SelectItemId(FName ItemId) const
{
	return ItemId;
}

void UCrimeGameInstanceSubsystem::ShowCenterMessage(const FString& Message)
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController)
	{
		return;
	}

	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();
	if (!IsValid(CrimeHUD))
	{
		return;
	}

	CrimeHUD->ShowCenterMessage(Message);
}

void UCrimeGameInstanceSubsystem::OnFadeEffect()
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController)
	{
		return;
	}

	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();
	if (!IsValid(CrimeHUD))
	{
		return;
	}

	CrimeHUD->OnFadeEffect();
}

void UCrimeGameInstanceSubsystem::SetPlayerControlEnabled(bool bEnabled)
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController)
	{
		return;
	}

	const bool bIgnoreInput = !bEnabled;
	PlayerController->SetIgnoreMoveInput(bIgnoreInput);
	PlayerController->SetIgnoreLookInput(bIgnoreInput);
}