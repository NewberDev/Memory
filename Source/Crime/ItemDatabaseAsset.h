// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDatabaseAsset.generated.h"


class UTexture2D;
class UUserWidget;

UENUM(BlueprintType)
enum class EItemUseAction : uint8
{
	None        UMETA(DisplayName = "None"),
	ToggleLight UMETA(DisplayName = "Toggle Light")
};

USTRUCT(BlueprintType)
struct FInventoryItem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (MultiLine = "true"))
	FString Hint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Inspect")
	TSubclassOf<UUserWidget> InspectWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Use")
	EItemUseAction UseAction = EItemUseAction::None;
};
/**
 * 
 */
UCLASS(BlueprintType)
class CRIME_API UItemDatabaseAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Data")
	TArray<FInventoryItem> Items;
	
};
