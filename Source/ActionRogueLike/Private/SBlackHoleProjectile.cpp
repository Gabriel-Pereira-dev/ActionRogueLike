 // Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHoleProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

void ASBlackHoleProjectile::OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp,Warning,TEXT("Overlapping %s"), *OtherActor->GetName());
     if(OtherComp->IsSimulatingPhysics()){
     	UE_LOG(LogTemp,Warning,TEXT("Destroying %s"), *OtherActor->GetName());
		OtherActor->Destroy();
     }
	
}

void ASBlackHoleProjectile::BeginPlay()
 {
	Super::BeginPlay();

	//SphereComp->IgnoreActorWhenMoving(GetInstigator(),true);
	//SetLifeSpan(5.0f);
 }
 
void ASBlackHoleProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RadialForceComp->FireImpulse();
}

 ASBlackHoleProjectile::ASBlackHoleProjectile()
 {
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>("RadialForceComp");
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->ForceStrength = 2000.0f;
	RadialForceComp->Radius = 700.0f;
	RadialForceComp->bIgnoreOwningActor = true;
	
	SphereComp->SetSphereRadius(20.0f);

	MovementComp->InitialSpeed = 20.0f;

	SphereComp->SetCollisionProfileName("BlackHole");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHoleProjectile::OnSphereCompBeginOverlap);

  
 }
