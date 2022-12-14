// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class USHealthComponent;
class URadialForceComponent;
class UParticleSystem;
class UMaterialInterface;

UCLASS()
class GEARSLIKESHOOTER_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USHealthComponent* healthComp;
	UPROPERTY(VisibleAnywhere, Category = "Components")
	URadialForceComponent* radialForceComp;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* PassedHealthComp, float Health, float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing=OnRep_Exploaded)
	bool exploded;
	UFUNCTION()
	void OnRep_Exploaded();
	
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	float explosionImpulse;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* explosionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UMaterialInterface* explodedMaterial;
};
