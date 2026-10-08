// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemDatabaseAsset.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInventoryItemAdded,
	FInventoryItem,
	Item
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnInventoryItemRemoved,
	FInventoryItem,
	Item
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIME_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	// 해당 액터에 붙어 있는 인벤토리를 찾는다.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static UInventoryComponent* FindInventory(AActor* OwnerActor);

	// 인벤토리를 직접 가져오지 않고 보유 여부를 확인한다.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	static bool HasItemOnActor(AActor* OwnerActor, FName ItemId);

	// 동일 ID는 한 번만 보관한다.
	// 실제 추가됐을 때만 true를 반환한다.
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(const FInventoryItem& Item);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem(FName ItemId) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemId);

	// UI가 표시할 목록을 가져온다.
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FInventoryItem> GetItems() const;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemAdded OnInventoryItemAdded;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryItemRemoved OnInventoryItemRemoved;

private:
	// 외부에서는 AddItem / RemoveItem으로만 변경한다.
	UPROPERTY(Transient)
	TArray<FInventoryItem> OwnedItems;

		
};
