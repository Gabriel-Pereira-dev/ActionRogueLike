// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackHoleProjectile.generated.h"

class URadialForceComponent;
/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API ASBlackHoleProjectile : public ASProjectileBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	URadialForceComponent* RadialForceComp;

	FTimerHandle* TimerHandle_BlackHolePull;

	UFUNCTION()
	void OnSphereCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

public:	
	// Sets default values for this actor's properties
	ASBlackHoleProjectile();
};
