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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// [추가] 아이템과 동일한 상호작용 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
	TObjectPtr<UInteractionComponent> InteractionComponent;

	// [추가] 블루프린트에서 구현할 상호작용 이벤트
	UFUNCTION(BlueprintImplementableEvent, Category = "Door|Interaction")
	void OnDoorInteracted(AActor* InteractingActor);

private:
	// [추가] 컴포넌트의 델리게이트에 바인딩할 함수
	UFUNCTION()
	void HandleInteraction(AActor* InteractingActor);

	// [삭제] 기존 Tick 선언은 사용하지 않으므로 제거
};
