// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FInteractionDelegate,
	AActor*, InteractingActor
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRIME_API UInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

    // Interaction Component를 찾기 위한 Static함수.
    UFUNCTION(BlueprintPure, Category = "Interaction")
    static UInteractionComponent* FindInteractionComponent(AActor* TargetActor);

    // 플레이어가 감지한 Interactor에서 실행될 함수.
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void OnInteraction(AActor* InteractingActor);

    // 실제 Interactor에서 실행될 로직을 바인드하는 함수.
    UPROPERTY(BlueprintAssignable, Category = "Interaction")
    FInteractionDelegate OnInteractionRequested;

		
};
