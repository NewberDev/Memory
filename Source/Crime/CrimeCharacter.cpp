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
}

void ACrimeCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    APlayerController* PlayerController =
        Cast<APlayerController>(NewController);

    if (!PlayerController)
    {
        return;
    }

    ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
    if (!LocalPlayer)
    {
        return;
    }

    UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
            LocalPlayer);

    if (!InputSubsystem || !DefaultMappingContext)
    {
        return;
    }

    // [추가] 플레이어가 캐릭터를 소유하는 시점에 컨텍스트 등록
    InputSubsystem->AddMappingContext(DefaultMappingContext, 0);
}

// Called every frame
void ACrimeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
    LookInput = Value.Get<FVector2D>();

    // 추후 추가:
    // LookInput을 이용해 라이트의 조준 방향을 갱신한다.
    //
    // AddControllerYawInput / AddControllerPitchInput은 호출하지 않는다.
}

void ACrimeCharacter::StopLook(const FInputActionValue& Value)
{
    // 마우스를 멈췄을 때 이전 이동량이 남지 않도록 초기화
    LookInput = FVector2D::ZeroVector;
}

void ACrimeCharacter::Interact()
{
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
}

void ACrimeCharacter::StopDash()
{
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    GetCharacterMovement()->RotationRate.Yaw = WalkRotationRate;
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