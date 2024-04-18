// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USInteractionComponent;
class UCameraComponent;
class USpringArmComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
		class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Mouse Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MouseAction;

	/** Primary Attack Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PrimaryAttackAction;
	
	/** Primary Attack Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** Blackhole Attack Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* BlackHoleAction;

	/** Teleport Attack Input Action */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category= Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TeleportAction;
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category= Attack)
	TSubclassOf<AActor> MagicProjectileClass;

	UPROPERTY(EditAnywhere, Category= Attack)
	TSubclassOf<AActor> BlackHoleProjectileClass;

	UPROPERTY(EditAnywhere, Category= Attack)
	TSubclassOf<AActor> TeleportProjectileClass;

	UPROPERTY(EditAnywhere, Category= Attack)
	UAnimMontage* PrimaryAttackAnim;

	FTimerHandle TimerHandle_PrimaryAttack;

protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent* InteractionComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category= "Components")
	USAttributeComponent* AttributeComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Movement Input
	void Move(const FInputActionValue& Value);

	// Mouse Input
	void Mouse(const FInputActionValue& Value);

	// Primary Attack Input
	void PrimaryAttack(const FInputActionValue& Value);

	// Primary Attack After Time Elapsed
	void PrimaryAttack_TimeElapsed();

	// Primary Interact  Input
	void PrimaryInteract(const FInputActionValue& Value);

	// BlackHole Attack Input
	void BlackHoleAttack(const FInputActionValue& Value);

	// BlackHole Attack After Time Elapsed
	void BlackHoleAttack_TimeElapsed();

	// Teleport Attack Input
	void TeleportAttack(const FInputActionValue& Value);

	// Teleport Attack After Time Elapsed
	void TeleportAttack_TimeElapsed();

	void SpawnProjectile(TSubclassOf<AActor> ProjectileClass);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
