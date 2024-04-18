// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "Animation/AnimMontage.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;
	JumpMaxCount = 1;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Mapping Context
	if(APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext,0);
		}
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if(UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASCharacter::Move);
		EnhancedInputComponent->BindAction(MouseAction,ETriggerEvent::Triggered, this, &ASCharacter::Mouse);
		EnhancedInputComponent->BindAction(PrimaryAttackAction,ETriggerEvent::Triggered, this, &ASCharacter::PrimaryAttack);
		EnhancedInputComponent->BindAction(JumpAction,ETriggerEvent::Started, this, &ASCharacter::Jump);
		EnhancedInputComponent->BindAction(InteractAction,ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);
		EnhancedInputComponent->BindAction(BlackHoleAction,ETriggerEvent::Triggered, this, &ASCharacter::BlackHoleAttack);
		EnhancedInputComponent->BindAction(TeleportAction,ETriggerEvent::Triggered, this, &ASCharacter::TeleportAttack);
	}

}

void ASCharacter::Move(const FInputActionValue& Value)
{
	// Input as Vector2D
	FVector2D InputValue = Value.Get<FVector2D>();

	// Get Camera Controller Rotations
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Roll = 0.0f;
	ControlRotation.Pitch = 0.0f;
	FVector ForwardMovementVector = ControlRotation.Vector();
	FVector RightMovementVector = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::Y);
	
	// Add Input
	AddMovementInput(ForwardMovementVector,InputValue.Y);
	AddMovementInput(RightMovementVector,InputValue.X);
}

void ASCharacter::Mouse(const FInputActionValue& Value)
{
	// Input as Vector2D
	FVector2D InputValue = Value.Get<FVector2D>();
	
	// Add Input
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-1 * InputValue.Y);
}

void ASCharacter::PrimaryAttack(const FInputActionValue& Value)
{
	
	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack);
	//float AnimPlayLength = PrimaryAttackAnim->GetPlayLength();
	//bool IsPlayingAnimMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(PrimaryAttackAnim);

	if(!bIsTimerActive)
	{
		// Play Anim
		PlayAnimMontage(PrimaryAttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2);
	}

}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	if(ensure((MagicProjectileClass)))
	{
		SpawnProjectile(MagicProjectileClass);
		
	}
}

void ASCharacter::PrimaryInteract(const FInputActionValue& Value)
{
	InteractionComp->PrimaryInteract();
}

void ASCharacter::BlackHoleAttack(const FInputActionValue& Value)
{
	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack);
	//float AnimPlayLength = PrimaryAttackAnim->GetPlayLength();
	//bool IsPlayingAnimMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(PrimaryAttackAnim);

	if(!bIsTimerActive)
	{
		// Play Anim
		PlayAnimMontage(PrimaryAttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2);
	}
}

void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	if(ensure((BlackHoleProjectileClass)))
	{
		SpawnProjectile(BlackHoleProjectileClass);
		
	}
}

void ASCharacter::TeleportAttack(const FInputActionValue& Value)
{
	bool bIsTimerActive = GetWorldTimerManager().IsTimerActive(TimerHandle_PrimaryAttack);
	//float AnimPlayLength = PrimaryAttackAnim->GetPlayLength();
	//bool IsPlayingAnimMontage = GetMesh()->GetAnimInstance()->Montage_IsPlaying(PrimaryAttackAnim);

	if(!bIsTimerActive)
	{
		// Play Anim
		PlayAnimMontage(PrimaryAttackAnim);

		GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::TeleportAttack_TimeElapsed, 0.2);
	}
	
}

void ASCharacter::TeleportAttack_TimeElapsed()
{
	if(ensure((TeleportProjectileClass)))
	{
		SpawnProjectile(TeleportProjectileClass);
		
	}

}

void ASCharacter::SpawnProjectile(TSubclassOf<AActor> ProjectileClass)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	
	FHitResult Hit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + ( CameraComp->GetForwardVector() * 8000);
	
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit,Start,End,ObjectQueryParams);
	//DrawDebugLine(GetWorld(),Start,End,FColor::Magenta,false,2.0,0,2.0);
	
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FVector ProjectileTarget = bBlockingHit ? Hit.ImpactPoint : End;
	FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(HandLocation,ProjectileTarget);
	
	FTransform SpawnTM = FTransform(ProjectileRotation,HandLocation);

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(ProjectileClass,SpawnTM,SpawnParameters);
}
