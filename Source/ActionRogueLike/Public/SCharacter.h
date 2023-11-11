// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "SCharacter.generated.h"

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
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category= Attack)
	TSubclassOf<AActor> ProjectileClass;

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
