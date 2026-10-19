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
	TObjectPtr<UInputAction> LightAction;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Input")
	FVector2D LookInput = FVector2D::ZeroVector;

	UFUNCTION(BlueprintImplementableEvent, Category = "Light")
	void LightOn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Light")
	void LightOff();

public:	

	virtual void PossessedBy(AController* NewController) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsLightOn() const { return bIsLightOn; }
private:
	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void StopLook(const FInputActionValue& Value);

	void Interact();

	void StartDash();
	void StopDash();

	void ToggleLight();

	void ShowInventoryItemHint(int32 ItemIndex);

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

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Light",
		meta = (AllowPrivateAccess = "true"))
	bool bIsLightOn = true;
};
