// Fill out your copyright notice in the Description page of Project Settings.


#include "CrimeCharacter.h"

#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h" 
#include "InputActionValue.h" 
#include "Components/SphereComponent.h"
#include "InteractionComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CrimeGameInstanceSubsystem.h"
#include "Components/InputComponent.h" 
#include "InputCoreTypes.h"             
#include "Framework/Commands/InputChord.h"
#include "Animation/AnimInstance.h"

// Sets default values
ACrimeCharacter::ACrimeCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InteractionSphere =
        CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));

    InteractionSphere->SetupAttachment(GetRootComponent());
    InteractionSphere->InitSphereRadius(200.0f);
    InteractionSphere->SetCollisionEnabled(
        ECollisionEnabled::QueryOnly);

    InventoryComponent =
        CreateDefaultSubobject<UInventoryComponent>(
            TEXT("PlayerInventory"));

}

// Called when the game starts or when spawned
void ACrimeCharacter::BeginPlay()
{
	Super::BeginPlay();

    bIsLightOn = true;

    GetCharacterMovement()->RotationRate.Yaw = WalkRotationRate;

    if (!IsValid(InventoryComponent))
    {
        InventoryComponent = FindComponentByClass<UInventoryComponent>();
    }
}

void ACrimeCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    APlayerController* PlayerController =
        Cast<APlayerController>(NewController);

    if (!PlayerController) return;

    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    if (!LocalPlayer) return;

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            LocalPlayer);

    if (!InputSubsystem || !DefaultMappingContext) return;

    // [추가] 플레이어가 캐릭터를 소유하는 시점에 컨텍스트 등록
    InputSubsystem->AddMappingContext(DefaultMappingContext, 0);

    PlayerController->bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);
}

// Called every frame
void ACrimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsBodyLookHeld)
    {
        UpdateBodyLookFromMouseCursor();
    }

    BodyLookYaw = FMath::FInterpTo(
        BodyLookYaw,
        TargetBodyLookYaw,
        DeltaTime,
        BodyLookInterpSpeed);

    if (FMath::IsNearlyEqual(BodyLookYaw, TargetBodyLookYaw, 0.01f))
    {
        BodyLookYaw = TargetBodyLookYaw;
    }
}

// Called to bind functionality to input
void ACrimeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInput =
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (!ensureMsgf(EnhancedInput,
        TEXT("CrimeCharacter requires EnhancedInputComponent.")))
    {
        return;
    }

    // [추가] 이동 액션 바인딩
    if (ensureMsgf(MoveAction, TEXT("MoveAction is not assigned.")))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACrimeCharacter::Move);
    }

    // [추가] 마우스 입력 바인딩
    if (ensureMsgf(LookAction, TEXT("LookAction is not assigned.")))
    {
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACrimeCharacter::Look);

    
        // 입력이 끝나면 저장된 이동량 초기화
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Completed, this, &ACrimeCharacter::StopLook);

        EnhancedInput->BindAction(LookAction, ETriggerEvent::Canceled, this, &ACrimeCharacter::StopLook);
    }

    if (ensureMsgf(
        BodyLookHoldAction,
        TEXT("BodyLookHoldAction is not assigned.")))
    {
        EnhancedInput->BindAction( BodyLookHoldAction, ETriggerEvent::Started, this, &ACrimeCharacter::StartBodyLookHold);

        EnhancedInput->BindAction( BodyLookHoldAction, ETriggerEvent::Completed, this, &ACrimeCharacter::StopBodyLookHold);

        EnhancedInput->BindAction( BodyLookHoldAction, ETriggerEvent::Canceled, this, &ACrimeCharacter::StopBodyLookHold);
    }


    if (ensureMsgf(InteractionAction, TEXT("InteractionAction is not assigned.")))
    {
        EnhancedInput->BindAction(InteractionAction, ETriggerEvent::Started, this, &ACrimeCharacter::Interact);
    }

    if (ensureMsgf(DashAction, TEXT("DashAction is not assigned.")))
    {
        // 누르는 순간: 450
        EnhancedInput->BindAction(DashAction, ETriggerEvent::Started, this, &ACrimeCharacter::StartDash);

        // 놓는 순간: 230
        EnhancedInput->BindAction(DashAction, ETriggerEvent::Completed, this, &ACrimeCharacter::StopDash);

        // 입력 취소 시에도 기본 속도로 복귀
        EnhancedInput->BindAction(DashAction, ETriggerEvent::Canceled, this, &ACrimeCharacter::StopDash);
    }

    if (ensureMsgf(SettingsAction,
        TEXT("SettingsAction is not assigned.")))
    {
        EnhancedInput->BindAction(
            SettingsAction,
            ETriggerEvent::Started,
            this,
            &ACrimeCharacter::OpenSettings);
    }

    if (ensureMsgf(ShowControlsAction,
        TEXT("ShowControlsAction is not assigned.")))
    {
        EnhancedInput->BindAction(
            ShowControlsAction,
            ETriggerEvent::Started,
            this,
            &ACrimeCharacter::ShowControls);
    }

    const FKey HintKeys[] =
    {
        EKeys::One,
        EKeys::Two,
        EKeys::Three,
        EKeys::Four,
        EKeys::Five,
        EKeys::Six
    };

    for (int32 ItemIndex = 0; ItemIndex < 6; ++ItemIndex)
    {
        FInputKeyBinding HintBinding(
            FInputChord(HintKeys[ItemIndex]),
            IE_Pressed);

        // 키를 누르면 해당 인덱스를 함수에 전달
        HintBinding.KeyDelegate.GetDelegateForManualSet().BindUObject(
            this,
            &ACrimeCharacter::ShowInventoryItemHint,
            ItemIndex);

        PlayerInputComponent->KeyBindings.Add(MoveTemp(HintBinding));
    }

    if (ensureMsgf(
        InventorySelectAction,
        TEXT("InventorySelectAction is not assigned.")))
    {
        // [수정] 누름 → GamepadSelectInventory(true)
        EnhancedInput->BindAction(
            InventorySelectAction,
            ETriggerEvent::Started,
            this,
            &ACrimeCharacter::GamepadSelectInventory,
            true);

        // [수정] 놓음 → GamepadSelectInventory(false)
        EnhancedInput->BindAction(
            InventorySelectAction,
            ETriggerEvent::Completed,
            this,
            &ACrimeCharacter::GamepadSelectInventory,
            false);
    }

    if (ensureMsgf(
        InventoryNavigateAction,
        TEXT("InventoryNavigateAction is not assigned.")))
    {
        // [수정] 좌우 → GamepadNavigateInventory
        EnhancedInput->BindAction(
            InventoryNavigateAction,
            ETriggerEvent::Triggered,
            this,
            &ACrimeCharacter::GamepadNavigateInventory);
    }
}

void ACrimeCharacter::CancelDash()
{
    if (bIsDashing) StopDash();

    OnBodyLookYawResetRequested();
}

void ACrimeCharacter::CancelBodyLookHoldForUI()
{
    StopBodyLookHold();
}

void ACrimeCharacter::CancelGamepadInventorySelection()
{
    bIsSelectingInventory = false;
}

void ACrimeCharacter::Move(const FInputActionValue& Value)
{
    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!PlayerController)
    {
        return;
    }

    const FVector2D MovementInput = Value.Get<FVector2D>();

    // 실제 플레이어 카메라의 방향을 가져온다.
    FVector ViewLocation;
    FRotator ViewRotation;
    PlayerController->GetPlayerViewPoint(ViewLocation, ViewRotation);

    // 탑뷰 카메라의 아래쪽 기울기는 제외하고 수평 방향만 사용한다.
    const FRotator YawRotation(0.0f, ViewRotation.Yaw, 0.0f);

    const FVector ForwardDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

    const FVector RightDirection =
        FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

    // 입력 Y = 앞뒤, 입력 X = 좌우
    AddMovementInput(ForwardDirection, MovementInput.Y);
    AddMovementInput(RightDirection, MovementInput.X);
}

void ACrimeCharacter::Look(const FInputActionValue& Value)
{

    if (bIsDashing || bIsBodyLookHeld) return;

    UAnimInstance* AnimInstance =
        GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

    if (AnimInstance && AnimInstance->IsAnyMontagePlaying())
    {
        return;
    }

    LookInput = Value.Get<FVector2D>();

    float HorizontalInput = LookInput.X;

    if (APlayerController* PlayerController =
        Cast<APlayerController>(GetController()))
    {
        const FVector CameraForward =
            PlayerController->GetControlRotation().Vector();

        const FVector CharacterForward = GetActorForwardVector();

        // 카메라 정면이 캐릭터 뒤쪽 반구에 있을 때만 좌우 입력을 반전한다.
        if (FVector::DotProduct(CameraForward, CharacterForward) < 0.0f)
        {
            HorizontalInput *= -1.0f;
        }
    }

    const UWorld* World = GetWorld();
    const float DeltaTime = World ? World->GetDeltaSeconds() : 0.0f;

    // 게임패드 입력을 초당 회전량으로 적용한다.
    TargetBodyLookYaw += HorizontalInput * BodyLookSensitivity * DeltaTime;

    //TargetBodyLookYaw += HorizontalInput * BodyLookSensitivity;

    TargetBodyLookYaw = FMath::Clamp(
        TargetBodyLookYaw,
        -MaxBodyLookYaw,
        MaxBodyLookYaw);


    /*
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(
            1001,                 // 매번 같은 키를 사용해 한 줄을 갱신
            0.0f,                 // 다음 프레임까지 표시
            FColor::Yellow,
            FString::Printf(
                TEXT("BodyLookYaw: %.2f deg | Mouse X: %.2f"),
                BodyLookYaw,
                LookInput.X));
    }*/

}

void ACrimeCharacter::StopLook(const FInputActionValue& Value)
{
    // 이동이 멈췄을때, 초기화.
    LookInput = FVector2D::ZeroVector;
}

void ACrimeCharacter::Interact()
{
    if (bIsSelectingInventory) return;
    if (!bIsLightOn) return;
    if (!IsValid(InteractionSphere)) return;


    TArray<AActor*> OverlappingActors;
    InteractionSphere->GetOverlappingActors(OverlappingActors);

    const FVector PlayerLocation = GetActorLocation();

    const FVector ForwardDirection =
        GetActorForwardVector().GetSafeNormal2D();

    const float MinFacingDot = FMath::Cos(
        FMath::DegreesToRadians(InteractionHalfAngleDegrees));

    const FVector InteractionOrigin =
        PlayerLocation
        - ForwardDirection * InteractionOriginBackwardOffset;

    for (AActor* TargetActor : OverlappingActors)
    {
        if (!IsValid(TargetActor) || TargetActor == this)
        {
            continue;
        }

        UInteractionComponent* TargetInteraction =
            UInteractionComponent::FindInteractionComponent(TargetActor);

        if (!IsValid(TargetInteraction)) continue;

        const FVector DirectionToTarget =
            (TargetActor->GetActorLocation() - InteractionOrigin)
            .GetSafeNormal2D();

        const float FacingDot =
            FVector::DotProduct(ForwardDirection, DirectionToTarget);

        if (FacingDot > MinFacingDot)
        {
            TargetInteraction->OnInteraction(this);
            break;
        }
    }
}

void ACrimeCharacter::StartDash()
{
    GetCharacterMovement()->MaxWalkSpeed = DashSpeed;

    GetCharacterMovement()->RotationRate.Yaw = DashRotationRate;

    bIsDashing = true;

    StopBodyLookHold();
}

void ACrimeCharacter::StopDash()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    GetCharacterMovement()->RotationRate.Yaw = WalkRotationRate;

    bIsDashing = false;
}

void ACrimeCharacter::ToggleLight()
{
    UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this);

    if (!IsValid(Subsystem)) return;

    UInventoryComponent* PlayerInventory =
        Subsystem->GetPlayerInventory();

    if (!IsValid(PlayerInventory)) return;

    static const FName HandLightItemId(TEXT("HandLight"));

    if (!PlayerInventory->HasItem(HandLightItemId)) return;

    if (bIsLightOn)
    {
        bIsLightOn = false;
        LightOff();
    }
    else
    {
        bIsLightOn = true;
        LightOn();
    }

    Subsystem->RefreshInteracterKeyVisibility();
}

void ACrimeCharacter::ShowInventoryItemHint(int32 ItemIndex)
{
    UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this);

    if (!IsValid(Subsystem)) return;

    Subsystem->InspectInventoryItem(ItemIndex);
}

void ACrimeCharacter::OpenSettings()
{
    if (UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this))
    {
        Subsystem->OpenSettings();
    }
}

void ACrimeCharacter::ShowControls()
{
    UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this);

    if (!IsValid(Subsystem))
    {
        return;
    }

    Subsystem->ShowControls();
}


void ACrimeCharacter::UpdateBodyLookFromMouseCursor()
{
    if (bIsDashing) return;

    UAnimInstance* AnimInstance =
        GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;

    if (AnimInstance && AnimInstance->IsAnyMontagePlaying())return;

    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!IsValid(PlayerController)) return;

    FVector RayOrigin;
    FVector RayDirection;

    if (!PlayerController->DeprojectMousePositionToWorld(
        RayOrigin,
        RayDirection))
    {
        return;
    }
    // 마우스 광선과 캐릭터 높이의 수평면이 만나는 지점을 구한다.
    if (FMath::IsNearlyZero(RayDirection.Z)) return;

    const float DistanceToPlayerHeight =
        (GetActorLocation().Z - RayOrigin.Z) / RayDirection.Z;

    if (DistanceToPlayerHeight <= 0.0f) return;

    const FVector MouseWorldPoint =
        RayOrigin + RayDirection * DistanceToPlayerHeight;

    FVector DirectionToMouse =
        MouseWorldPoint - GetActorLocation();

    DirectionToMouse.Z = 0.0f;

    if (!DirectionToMouse.Normalize()) return;

    FVector CharacterForward = GetActorForwardVector();
    CharacterForward.Z = 0.0f;

    if (!CharacterForward.Normalize()) return;

    const float CrossZ =
        FVector::CrossProduct(CharacterForward, DirectionToMouse).Z;

    const float Dot =
        FVector::DotProduct(CharacterForward, DirectionToMouse);

    const float CursorAngle =
        FMath::RadiansToDegrees(FMath::Atan2(CrossZ, Dot));

  
    constexpr float MouseAngleLimit = 60.0f;
    constexpr float BodyYawLimit = 20.0f;

    const float ClampedCursorAngle =
        FMath::Clamp(CursorAngle, -MouseAngleLimit, MouseAngleLimit);

    const float AngleToBodyYawRatio =
        BodyYawLimit / MouseAngleLimit;

    TargetBodyLookYaw =
        ClampedCursorAngle * AngleToBodyYawRatio;
}

void ACrimeCharacter::StartBodyLookHold()
{
    if (bIsDashing) return;

    bIsBodyLookHeld = true;

    if (APlayerController* PlayerController =
        Cast<APlayerController>(GetController()))
    {
        PlayerController->bShowMouseCursor = true;

        FInputModeGameAndUI InputMode;
        InputMode.SetHideCursorDuringCapture(false);
        InputMode.SetLockMouseToViewportBehavior(
            EMouseLockMode::DoNotLock);

        PlayerController->SetInputMode(InputMode);
    }

    // 버튼을 누른 순간 커서 위치도 반영한다.
    UpdateBodyLookFromMouseCursor();
}

void ACrimeCharacter::StopBodyLookHold()
{
    bIsBodyLookHeld = false;
    TargetBodyLookYaw = 0.0f;

    APlayerController* PlayerController =
        Cast<APlayerController>(GetController());

    if (!IsValid(PlayerController)) return;

    // 퍼즐 UI가 입력을 소유 중이면 UIOnly 모드와 커서 상태를 유지한다.
    if (PlayerController->IsLookInputIgnored()) return;

    PlayerController->bShowMouseCursor = false;

    FInputModeGameOnly InputMode;
    PlayerController->SetInputMode(InputMode);
}

void ACrimeCharacter::GamepadSelectInventory(bool bSelecting)
{
    UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this);

    if (!IsValid(Subsystem)) return;

    if (bSelecting)
    {
        // RB 누름
        if (bIsSelectingInventory) return;
        if (!IsValid(InventoryComponent)) return;
        if (InventoryComponent->GetItems().IsEmpty()) return;


        bIsSelectingInventory = true;
        SelectedInventoryIndex = 0;

        // 선택 표시 켜기
        Subsystem->SetInventorySelectionVisible(true);
    }
    else
    {
        // RB 놓음
        if (!bIsSelectingInventory) return;

        bIsSelectingInventory = false;

        // 선택 표시 끄기
        Subsystem->SetInventorySelectionVisible(false);

        // 선택한 아이템 실행
        Subsystem->InspectInventoryItem(SelectedInventoryIndex);
    }
}

void ACrimeCharacter::GamepadNavigateInventory(const FInputActionValue& Value)
{

    if (!bIsSelectingInventory) return;

    if (!IsValid(InventoryComponent)) return;


    // 실제 소지 아이템 수
    const int32 ItemCount = InventoryComponent->GetItems().Num();

    if (ItemCount == 0) return;

    const float AxisValue = Value.Get<float>();

    if (FMath::IsNearlyZero(AxisValue)) return;

    // 왼쪽 -1 / 오른쪽 +1
    const int32 Direction = AxisValue > 0.0f ? 1 : -1;

    // 소지 아이템 범위 안에서만 이동
    const int32 NewIndex = FMath::Clamp(
        SelectedInventoryIndex + Direction,
        0,
        ItemCount - 1);

    if (NewIndex == SelectedInventoryIndex) return;

    SelectedInventoryIndex = NewIndex;

    if (UCrimeGameInstanceSubsystem* Subsystem =
        UCrimeGameInstanceSubsystem::Get(this))
    {
        // 선택 표시 위치만 변경
        Subsystem->SetInventorySelectionIndex(SelectedInventoryIndex);
    }
}
