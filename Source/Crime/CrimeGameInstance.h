// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CrimeGameInstance.generated.h"

class USoundClass;
class USoundMix;

/**
 * 
 */
UCLASS()
class CRIME_API UCrimeGameInstance : public UGameInstance
{
	GENERATED_BODY()


private:
    UPROPERTY(VisibleAnywhere, Category = "Settings|Audio")
    float MasterVolume = 100.0f;

    // SC_Master 지정
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Audio")
    TObjectPtr<USoundClass> MasterSoundClass;

    // SM_MasterVolume 지정
    UPROPERTY(EditDefaultsOnly, Category = "Settings|Audio")
    TObjectPtr<USoundMix> MasterSoundMix;

public:
    UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
    void SetMasterVolume(float NewVolume);

    UFUNCTION(BlueprintPure, Category = "Settings|Audio")
    float GetMasterVolume() const;

    UFUNCTION(BlueprintCallable, Category = "Settings|Audio")
    void ApplyMasterVolume();


	
};
