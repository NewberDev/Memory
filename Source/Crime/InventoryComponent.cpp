// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "CrimeGameInstanceSubsystem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


UInventoryComponent* UInventoryComponent::FindInventory(AActor* OwnerActor)
{
	// 액터가 없거나 제거된 상태면 nullptr 반환.
	if (!IsValid(OwnerActor))
	{
		return nullptr;
	}

	return OwnerActor->FindComponentByClass<UInventoryComponent>();
}

bool UInventoryComponent::HasItemOnActor(AActor* OwnerActor, FName ItemId)
{
	// 컴포넌트 검색과 아이템 검사를 한 번에 처리.
	const UInventoryComponent* Inventory = FindInventory(OwnerActor);

	return Inventory && Inventory->HasItem(ItemId);
}

bool UInventoryComponent::AddItem(const FInventoryItem& Item)
{
	//빈 ID와 
	if (Item.ItemId.IsNone()) //일단 중복 방지는 제외한다.
	{
		return false;
	}

	if (OwnedItems.Num() >= MaxItemCount)
	{
		UCrimeGameInstanceSubsystem* Subsystem =
			UCrimeGameInstanceSubsystem::Get(this);

		if (IsValid(Subsystem))
		{
			Subsystem->ShowCenterMessageTextId(InventoryFullTextId);
		}

		return false;
	}

	OwnedItems.Add(Item);

	// UI : 획득 알림
	OnInventoryItemAdded.Broadcast(Item);
	// UI 인벤토리 알림.
	OnInventoryChanged.Broadcast();

	return true;
}

bool UInventoryComponent::HasItem(FName ItemId) const
{
	if (ItemId.IsNone())
	{
		return false;
	}

	return OwnedItems.ContainsByPredicate(
		[ItemId](const FInventoryItem& Item)
		{
			return Item.ItemId == ItemId;
		}
	);
}

bool UInventoryComponent::RemoveItem(FName ItemId)
{
	if (ItemId.IsNone()) return false;

	const int32 ItemIndex = OwnedItems.IndexOfByPredicate(
		[ItemId](const FInventoryItem& Item)
		{
			return Item.ItemId == ItemId;
		}
	);

	if (ItemIndex == INDEX_NONE) return false;

	const FInventoryItem RemovedItem = OwnedItems[ItemIndex];

	OwnedItems.RemoveAt(ItemIndex);

	//인벤토리에서 특정 아이템 삭제
	OnInventoryItemRemoved.Broadcast(RemovedItem);
	
	//인벤토리 바뀜.
	OnInventoryChanged.Broadcast();

	return true;
}

TArray<FInventoryItem> UInventoryComponent::GetItems() const
{
	return OwnedItems;
}

bool UInventoryComponent::CheckPickupSpace()
{
	if (OwnedItems.Num() < MaxItemCount)
	{
		return true;
	}

	// 인벤토리가 꽉 찼으면 기존 메시지 표시
	UCrimeGameInstanceSubsystem* Subsystem =
		UCrimeGameInstanceSubsystem::Get(this);

	if (IsValid(Subsystem))
	{
		Subsystem->ShowCenterMessageTextId(InventoryFullTextId);
	}

	return false;
}
