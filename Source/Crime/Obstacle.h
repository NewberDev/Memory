// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

class UInteractionComponent;

UCLASS()
class CRIME_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacle();

protected:

    UFUNCTION(BlueprintCallable, Category = "Obstacle")
    void InitializeObstacle(
        FName InRequiredItemId,
        UPARAM(meta = (GetOptions = "Crime.CrimeGameInstanceSubsystem.GetTextIdOptions"))
        FName InFailureTextId
    );

	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction")
    TObjectPtr<UInteractionComponent> InteractionComponent;

    //장애물 제거를 위해 필요한 ItemID
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
    FName RequiredItemId = NAME_None;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
    bool bShowFailureMessage = true;

    //장애물 제거를 실패할 경우 출력될 TextLog
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle",
        meta = (GetOptions = "Crime.CrimeGameInstanceSubsystem.GetTextIdOptions"))
    FName FailureTextId = NAME_None;

    //장애물 제거 조건 확인
    void TryRemoveObstacle(AActor* InteractingActor);

    //장애물 제거 실행 함수
    UFUNCTION(BlueprintNativeEvent, Category = "Obstacle")
    void RemoveObstacle(AActor* InteractingActor);

    virtual void RemoveObstacle_Implementation(AActor* InteractingActor);

private:
    // [추가] 컴포넌트의 델리게이트에 바인딩합니다.
    UFUNCTION()
    void HandleInteraction(AActor* InteractingActor);
};
