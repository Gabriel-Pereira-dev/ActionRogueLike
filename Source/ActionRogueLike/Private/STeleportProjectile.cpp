// Fill out your copyright notice in the Description page of Project Settings.


#include "STeleportProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

void ASTeleportProjectile::Explode_Implementation()
{
	
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedDetonation);

	FTimerHandle TimerHandle_TeleportDelay;

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVfx, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);
	
	GetWorldTimerManager().SetTimer(TimerHandle_TeleportDelay,this,&ASTeleportProjectile::TeleportInstigator, TeleportDelay);
	
}

void ASTeleportProjectile::TeleportInstigator()
{
	AActor* InstigatorActor = GetInstigator();
	if(ensure(InstigatorActor))
	{
		InstigatorActor->TeleportTo(GetActorLocation(),FQuat::Identity.Rotator(), false, false);
		Destroy();
	}

}

void ASTeleportProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	
	ASTeleportProjectile::Explode_Implementation();
}

void ASTeleportProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_DelayedDetonation, this,&ASTeleportProjectile::Explode, DetonateDelay);
	
}

ASTeleportProjectile::ASTeleportProjectile()
{
	TeleportDelay = 0.2f;
	DetonateDelay = 0.2f;
	
	MovementComp->InitialSpeed = 6000.0f;
}
