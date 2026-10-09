// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "CrimeGameInstanceSubsystem.h"

#include "InteractionComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(
			TEXT("InteractionComponent")
		);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(InteractionComponent))
	{
		InteractionComponent->OnInteractionRequested.AddUniqueDynamic(
			this,
			&AItem::HandleInteraction
		);
	}
	
}

void AItem::HandleInteraction(AActor* InteractingActor)
{
	// 기존 아이템 획득 로직 실행.
	TryPickup(InteractingActor);
}

bool AItem::TryPickup(AActor* InteractingActor)
{
	if (!IsValid(InteractingActor)) return false;


	UCrimeGameInstanceSubsystem* Subsystem =
		UCrimeGameInstanceSubsystem::Get(InteractingActor);

	if (!IsValid(Subsystem) ||
		!Subsystem->AddItemToPlayerInventory(ItemId))
	{
		return false;
	}

	Destroy();
	return true;
}




