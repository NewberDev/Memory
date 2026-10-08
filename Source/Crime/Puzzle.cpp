// Fill out your copyright notice in the Description page of Project Settings.


#include "Puzzle.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "CrimeGameInstanceSubsystem.h"

// Sets default values
APuzzle::APuzzle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void APuzzle::BeginPlay()
{
	Super::BeginPlay();
	
	if (!PuzzleWidgetClass)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		return;
	}

	// 인스턴스만 생성해 보관한다. 아직 화면에는 띄우지 않는다.
	PuzzleWidgetInstance =
		CreateWidget<UUserWidget>(PlayerController, PuzzleWidgetClass);

	if (IsValid(PuzzleWidgetInstance))
	{
		OnPuzzleWidgetCreated(PuzzleWidgetInstance);
	}

}

void APuzzle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 열려 있다면 위젯과 퍼즐 입력 상태를 정리한다.
	OffPuzzleScene();

	Super::EndPlay(EndPlayReason);
}

void APuzzle::OnPuzzleScene()
{
	if (!IsValid(PuzzleWidgetInstance) || PuzzleWidgetInstance->IsInViewport()) return;


	PuzzleWidgetInstance->AddToViewport();


	if (UCrimeGameInstanceSubsystem* Subsystem =
		UCrimeGameInstanceSubsystem::Get(this))
	{
		Subsystem->SetPuzzleSceneActive(true, PuzzleWidgetInstance);
	}

	OnPuzzleSceneOpened(PuzzleWidgetInstance);
}

void APuzzle::OffPuzzleScene()
{
	if (!IsValid(PuzzleWidgetInstance) ||
		!PuzzleWidgetInstance->IsInViewport()) return;

	// 화면에서만 떼고, PuzzleWidgetInstance 참조는 유지한다.
	PuzzleWidgetInstance->RemoveFromParent();

	if (UCrimeGameInstanceSubsystem* Subsystem =
		UCrimeGameInstanceSubsystem::Get(this))
	{
		Subsystem->SetPuzzleSceneActive(false, nullptr);
	}
}

void APuzzle::OnPuzzleWidgetCreated_Implementation(UUserWidget* PuzzleWidget)
{
	// 기본 퍼즐은 별도 생성 로직이 없다.
}

void APuzzle::OnPuzzleSceneOpened_Implementation(UUserWidget* PuzzleWidget)
{
	// 기본 퍼즐은 별도 시작 동작이 없다.
	// 자식 퍼즐 블루프린트에서 이 이벤트를 오버라이드한다.
}

