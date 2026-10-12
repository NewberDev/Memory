// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsDoor.h"
#include "InteractionComponent.h" 

// Sets default values
APhysicsDoor::APhysicsDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	InteractionComponent =
		CreateDefaultSubobject<UInteractionComponent>(
			TEXT("InteractionComponent")
		);
}

// Called when the game starts or when spawned
void APhysicsDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsValid(InteractionComponent))
	{
		InteractionComponent->OnInteractionRequested.AddUniqueDynamic(
			this,
			&APhysicsDoor::HandleInteraction
		);
	}
}

// [추가] 상호작용 요청을 블루프린트 이벤트로 전달
void APhysicsDoor::HandleInteraction(AActor* InteractingActor)
{
	if (!IsValid(InteractingActor))
	{
		return;
	}

	OnDoorInteracted(InteractingActor);
}

