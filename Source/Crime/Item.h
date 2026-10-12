// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"


class UInteractionComponent;

UCLASS(Abstract, Blueprintable)
class CRIME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	UFUNCTION(BlueprintCallable, Category = "Item")
	bool TryPickup(AActor* InteractingActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemId = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Pickup")
	bool bDestroyAfterPickup = true;

	// [추가] 인벤토리에 아이템이 정상 추가된 경우에만 실행.
	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Pickup")
	void OnPickupSucceeded(AActor* InteractingActor);

private:
	UFUNCTION()
	void HandleInteraction(AActor* InteractingActor);

	bool bPickupCompleted = false;

};
