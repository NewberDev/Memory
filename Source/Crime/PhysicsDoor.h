// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsDoor.generated.h"

class UInteractionComponent;

UCLASS()
class CRIME_API APhysicsDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APhysicsDoor();

	UFUNCTION(BlueprintCallable, Category = "Door|Interaction")
	void DoorUnlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	UFUNCTION(BlueprintImplementableEvent, Category = "Door|Interaction")
	void OnDoorInteracted(AActor* InteractingActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Interaction")
	bool bIsUnlock = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Interaction")
	bool bShowLockedMessage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door|Interaction",
		meta = (
			EditCondition = "bShowLockedMessage",
			GetOptions = "Crime.CrimeGameInstanceSubsystem.GetTextIdOptions"
			))
	FName LockedTextId = NAME_None;


private:

	UFUNCTION()
	void HandleInteraction(AActor* InteractingActor);

};
