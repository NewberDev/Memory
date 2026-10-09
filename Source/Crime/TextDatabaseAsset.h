// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TextDatabaseAsset.generated.h"


// 메시지 데이터 구조체.
USTRUCT(BlueprintType)
struct FMessageTextEntry
{
    GENERATED_BODY()

    // 메시지를 식별하는 ID.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text")
    FName TextId = NAME_None;

    // 실제 화면에 표시할 문구.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text",
        meta = (MultiLine = "true"))
    FText Message;
};

/**
 * 
 */
UCLASS()
class CRIME_API UTextDatabaseAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
    // [추가] 에셋에서 편집하는 메시지 목록.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Text Data",
        meta = (TitleProperty = "TextId"))
    TArray<FMessageTextEntry> Texts;
};
