// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrimeGameModeBase.generated.h"

class UItemDatabaseAsset;
class UTextDatabaseAsset;

/**
 * 
 */
UCLASS()
class CRIME_API ACrimeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Data")
	TObjectPtr<UItemDatabaseAsset> ItemDatabaseAsset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Data")
	TObjectPtr<UTextDatabaseAsset> TextDatabaseAsset = nullptr;

public:
	UItemDatabaseAsset* GetItemDatabaseAsset() const
	{
		return ItemDatabaseAsset;
	}
	
	UTextDatabaseAsset* GetTextDatabaseAsset() const
	{
		return TextDatabaseAsset;
	}
};
