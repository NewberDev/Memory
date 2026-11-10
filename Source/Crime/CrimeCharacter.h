// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "CrimeCharacter.generated.h"

class UInputMappingContext;
class UInputAction;      
struct FInputActionValue;
class USphereComponent;
class UInventoryComponent;

UCLASS()
class CRIME_API ACrimeCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACrimeCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InteractionAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> SettingsAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> ShowControlsAction;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Input")
	FVector2D LookInput = FVector2D::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> BodyLookHoldAction;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Look")
	bool bIsBodyLookHeld = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Light")
	void LightOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Light")
	void LightOff();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Look",
		meta = (AllowPrivateAccess = "true", Units = "Degrees"))
	float BodyLookYaw = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look",
		meta = (ClampMin = "0.0", Units = "Degrees"))
	float MaxBodyLookYaw = 20.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look",
		meta = (ClampMin = "0.0"))
	float BodyLookSensitivity = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look",
		meta = (ClampMin = "0.1"))
	float CursorLookInterpSpeed = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look",
		meta = (ClampMin = "0.0", ClampMax = "180.0", Units = "Degrees"))
	float MaxCursorLookAngle = 70.0f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Look")
	float TargetBodyLookYaw = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Look",
		meta = (ClampMin = "0.1"))
	float BodyLookInterpSpeed = 6.0f;

public:	

	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void ToggleLight();

	bool IsLightOn() const { return bIsLightOn; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Look")
	void OnBodyLookYawResetRequested();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void CancelDash();

	UFUNCTION(BlueprintCallable, Category = "Look")
	void CancelBodyLookHoldForUI();

private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StopLook(const FInputActionValue& Value);

	void Interact();

	void StartDash();
	void StopDash();

	void ShowInventoryItemHint(int32 ItemIndex);

	void OpenSettings();

	void ShowControls();

	void UpdateBodyLookFromMouseCursor();

	void StartBodyLookHold();
	void StopBodyLookHold();

	UPROPERTY(VisibleAnywhere, Category = "Interaction")
	TObjectPtr<USphereComponent> InteractionSphere;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Movement",
		meta = (ClampMin = "0.0"))
	float WalkSpeed = 230.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement",
		meta = (ClampMin = "0.0"))
	float DashSpeed = 450.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float WalkRotationRate = 200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float DashRotationRate = 400.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction",
		meta = (ClampMin = "0.0", ClampMax = "180.0", Units = "Degrees"))
	float InteractionHalfAngleDegrees = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction",
		meta = (ClampMin = "0.0", Units = "cm"))
	float InteractionOriginBackwardOffset = 30.0f;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Light",
		meta = (AllowPrivateAccess = "true"))
	bool bIsLightOn = true;

	bool bIsDashing = false;
};
