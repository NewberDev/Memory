// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"

#include "InteractionComponent.h"         
#include "InventoryComponent.h"           
#include "CrimeGameInstanceSubsystem.h"
// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    InteractionComponent =
        CreateDefaultSubobject<UInteractionComponent>(
            TEXT("InteractionComponent")
        );
}

void AObstacle::BeginPlay()
{
    Super::BeginPlay();

    // 이 장애물의 컴포넌트에 이 장애물의 함수만 바인딩합니다.
    if (IsValid(InteractionComponent))
    {
        InteractionComponent->OnInteractionRequested.AddUniqueDynamic(
            this,
            &AObstacle::HandleInteraction
        );
    }
}

void AObstacle::InitializeObstacle(
    FName InRequiredItemId,
    FName InFailureTextId)
{
    RequiredItemId = InRequiredItemId;
    FailureTextId = InFailureTextId;
}

void AObstacle::HandleInteraction(AActor* InteractingActor)
{
    TryRemoveObstacle(InteractingActor);
}

void AObstacle::TryRemoveObstacle(AActor* InteractingActor)
{
    if (!IsValid(InteractingActor))
    {
        return;
    }

    // ItemId가 비어 있으면 조건 미충족으로 처리한다.
    const bool bHasRequiredItem =
        !RequiredItemId.IsNone() &&
        UInventoryComponent::HasItemOnActor(
            InteractingActor,
            RequiredItemId
        );

    if (bHasRequiredItem)
    {
        RemoveObstacle(InteractingActor);
        return;
    }

    // 실패 문구가 등록되어 있을 때만 표시한다.
    if (!FailureTextId.IsNone())
    {
        UCrimeGameInstanceSubsystem* Subsystem =
            UCrimeGameInstanceSubsystem::Get(InteractingActor);

        if (IsValid(Subsystem))
        {
            Subsystem->ShowCenterMessageTextId(FailureTextId);
        }
    }
}

// [추가] 전체 함수
void AObstacle::RemoveObstacle_Implementation(AActor* InteractingActor)
{
    // 실제 장애물 제거 동작은 자식 블루프린트에서 구현한다.
}