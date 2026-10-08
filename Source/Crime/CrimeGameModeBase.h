// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CrimeGameModeBase.generated.h"

class UItemDatabaseAsset;

/**
 * 
 */
UCLASS()
class CRIME_API ACrimeGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item Data")
	TObjectPtr<UItemDatabaseAsset> ItemDatabaseAsset = nullptr;

public:
	UItemDatabaseAsset* GetItemDatabaseAsset() const
	{
		return ItemDatabaseAsset;
	}
	
};
