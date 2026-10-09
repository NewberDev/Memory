// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


UInteractionComponent* UInteractionComponent::FindInteractionComponent(AActor* TargetActor)
{
    if (!IsValid(TargetActor))
    {
        return nullptr;
    }

    return TargetActor->FindComponentByClass<UInteractionComponent>();
}


void UInteractionComponent::OnInteraction(AActor* InteractingActor)
{
    if (!IsValid(InteractingActor) || !IsValid(GetOwner()))
    {
        return;
    }

    // 바인딩된 아이템/퍼즐/장애물의 함수를 실행한다.
    OnInteractionRequested.Broadcast(InteractingActor);
}

