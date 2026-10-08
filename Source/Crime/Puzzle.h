// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puzzle.generated.h"

class UUserWidget;

UCLASS(Abstract)
class CRIME_API APuzzle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Puzzle|UI")
	TSubclassOf<UUserWidget> PuzzleWidgetClass;

	UPROPERTY(Transient)
	TObjectPtr<UUserWidget> PuzzleWidgetInstance = nullptr;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	

	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void OnPuzzleScene();

	UFUNCTION(BlueprintCallable, Category = "Puzzle|UI")
	void OffPuzzleScene();

	//위젯 클래스의 이벤트 종료시 내용과 바인드하기 위한실행 함수
	UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
	void OnPuzzleWidgetCreated(UUserWidget* PuzzleWidget);

	virtual void OnPuzzleWidgetCreated_Implementation(UUserWidget* PuzzleWidget);

	//퍼즐이 열릴때 인벤토리의 구성을 이 화면에 띄우기 위한 구성이다.
	UFUNCTION(BlueprintNativeEvent, Category = "Puzzle")
	void OnPuzzleSceneOpened(UUserWidget* PuzzleWidget);

	virtual void OnPuzzleSceneOpened_Implementation(UUserWidget* PuzzleWidget);

};
