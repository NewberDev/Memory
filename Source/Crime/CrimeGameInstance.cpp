// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimeGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "Engine/Engine.h"

void UCrimeGameInstance::SetMasterVolume(float NewVolume)
{
    if (!FMath::IsFinite(NewVolume))
    {
        return;
    }

    MasterVolume = FMath::Clamp(NewVolume, 0.0f, 100.0f);

    ApplyMasterVolume();
}

float UCrimeGameInstance::GetMasterVolume() const
{
    return MasterVolume;
}

void UCrimeGameInstance::ApplyMasterVolume()
{
    // 월드가 아직 없다면 저장된 값은 유지하고 적용만 보류
    if (!GetWorld()) return;


    if (!IsValid(MasterSoundClass) || !IsValid(MasterSoundMix))
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("ApplyMasterVolume: Assign MasterSoundClass and MasterSoundMix.")
        );
        return;
    }

    const float VolumeMultiplier = MasterVolume / 100.0f;

    // 실행 중 믹스의 볼륨 설정을 변경
    UGameplayStatics::SetSoundMixClassOverride(
        this,
        MasterSoundMix.Get(),
        MasterSoundClass.Get(),
        VolumeMultiplier,
        1.0f,   // Pitch 유지
        0.1f,   // 볼륨 전환 시간
        true    // 자식 클래스에도 적용
    );

    // 이 프로젝트의 기본 볼륨 제어 믹스로 활성화
    UGameplayStatics::SetBaseSoundMix(
        this,
        MasterSoundMix.Get()
    );
}