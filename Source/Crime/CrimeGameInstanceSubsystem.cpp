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
#include "TextDatabaseAsset.h" 
#include "CrimeCharacter.h"


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

	bInteracterKeyRequestedVisible = bVisible;

	RefreshInteracterKeyVisibility();
}

void UCrimeGameInstanceSubsystem::RefreshInteracterKeyVisibility()
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!IsValid(PlayerController))
	{
		return;
	}

	ACrimeHUD* CrimeHUD =
		PlayerController->GetHUD<ACrimeHUD>();

	if (!IsValid(CrimeHUD))
	{
		return;
	}

	const ACrimeCharacter* PlayerCharacter =
		Cast<ACrimeCharacter>(PlayerController->GetPawn());

	const bool bLightOn =
		IsValid(PlayerCharacter) && PlayerCharacter->IsLightOn();

	// [수정] 위에서 선언한 변수 사용
	const bool bShouldShow =
		bInteracterKeyRequestedVisible && bLightOn;

	CrimeHUD->SetInteracterKeyVisible(bShouldShow);
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

bool UCrimeGameInstanceSubsystem::CheckPlayerInventorySpace()
{
	UInventoryComponent* Inventory = GetPlayerInventory();

	if (!IsValid(Inventory))
	{
		return false;
	}

	return Inventory->CheckPickupSpace();
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

TArray<FString> UCrimeGameInstanceSubsystem::GetTextIdOptions()
{
	TArray<FString> Options;

	FAssetRegistryModule& AssetRegistryModule =
		FModuleManager::LoadModuleChecked<FAssetRegistryModule>(
			TEXT("AssetRegistry")
		);

	TArray<FAssetData> DatabaseAssets;
	AssetRegistryModule.Get().GetAssetsByClass(
		UTextDatabaseAsset::StaticClass()->GetClassPathName(),
		DatabaseAssets,
		true
	);

	for (const FAssetData& AssetData : DatabaseAssets)
	{
		const UTextDatabaseAsset* DatabaseAsset =
			Cast<UTextDatabaseAsset>(AssetData.GetAsset());

		if (!IsValid(DatabaseAsset))
		{
			continue;
		}

		for (const FMessageTextEntry& Entry : DatabaseAsset->Texts)
		{
			if (!Entry.TextId.IsNone())
			{
				Options.AddUnique(Entry.TextId.ToString());
			}
		}
	}

	Options.Sort();
	return Options;
}

FName UCrimeGameInstanceSubsystem::SelectTextId(FName TextId) const
{
	return TextId;
}

void UCrimeGameInstanceSubsystem::ShowCenterMessageText(
	const FString& Message)
{
	if (Message.IsEmpty()) return;

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World) return;


	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!IsValid(PlayerController)) return;


	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();

	if (!IsValid(CrimeHUD)) return;

	CrimeHUD->ShowCenterMessage(Message);
}

void UCrimeGameInstanceSubsystem::ShowCenterMessageTextId(FName TextId)
{

	if (TextId.IsNone()) return;

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World)
	{
		return;
	}

	const ACrimeGameModeBase* GameMode =
		World->GetAuthGameMode<ACrimeGameModeBase>();

	const UTextDatabaseAsset* DatabaseAsset =
		IsValid(GameMode) ? GameMode->GetTextDatabaseAsset() : nullptr;

	if (!IsValid(DatabaseAsset))
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("ShowCenterMessage: TextDatabaseAsset is not assigned.")
		);
		return;
	}

	const FMessageTextEntry* Entry =
		DatabaseAsset->Texts.FindByPredicate(
			[TextId](const FMessageTextEntry& Candidate)
			{
				return Candidate.TextId == TextId;
			}
		);

	if (!Entry)
	{
		UE_LOG(
			LogTemp,
			Warning,
			TEXT("ShowCenterMessage: TextId '%s' was not found."),
			*TextId.ToString()
		);
		return;
	}


	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController) return;

	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();
	if (!IsValid(CrimeHUD)) return;

	CrimeHUD->ShowCenterMessage(Entry->Message.ToString());
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

void UCrimeGameInstanceSubsystem::PlayInventoryHintAnimation(
	int32 InventoryIndex)
{
	if (InventoryIndex < 0) return;

	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!IsValid(GameInstance) || !IsValid(World)) return;

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!IsValid(PlayerController)) return;

	ACrimeHUD* CrimeHUD =
		PlayerController->GetHUD<ACrimeHUD>();

	if (!IsValid(CrimeHUD)) return;

	// HUD 블루프린트 이벤트 실행.
	CrimeHUD->PlayInventoryHintAnimation(InventoryIndex);
}

void UCrimeGameInstanceSubsystem::SetPlayerControlEnabled(bool bEnabled)
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!GameInstance || !World) return;

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!PlayerController) return;

	APawn* PlayerPawn = PlayerController->GetPawn();

	if (!IsValid(PlayerPawn)) return;

	if (bEnabled)
	{
		PlayerPawn->EnableInput(PlayerController);
	}
	else
	{
		PlayerPawn->DisableInput(PlayerController);
	}
}

void UCrimeGameInstanceSubsystem::HidePlayerBaseUI()
{
	UGameInstance* GameInstance = GetGameInstance();
	UWorld* World = GetWorld();

	if (!IsValid(GameInstance) || !IsValid(World))
	{
		return;
	}

	APlayerController* PlayerController =
		GameInstance->GetFirstLocalPlayerController(World);

	if (!IsValid(PlayerController))
	{
		return;
	}

	ACrimeHUD* CrimeHUD = PlayerController->GetHUD<ACrimeHUD>();

	if (!IsValid(CrimeHUD))
	{
		return;
	}

	CrimeHUD->HideBaseWidget();
}