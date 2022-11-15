// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

class USHealthComponent;
class USphereComponent;
class USoundCue;

UCLASS()
class GEARSLIKESHOOTER_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();
	void SelfDestruct();
	void DamageSelf();

	UFUNCTION()
	void HandleTakeDamage(USHealthComponent* PassedHealthComp, float Health, float HealthDelta,
		const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* healthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* sphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Stats", meta = (AllowPrivateAccess = "true"))
	float movementForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Stats", meta = (AllowPrivateAccess = "true"))
	bool useVelocityChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Stats", meta = (AllowPrivateAccess = "true"))
	float requiredDistanceToTarget;
	FVector nextPathPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Stats")
	float explosionRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Tracker Stats")
	float explostionDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FXs")
	UParticleSystem* explosionEffect;
	UMaterialInstanceDynamic* matInst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FXs")
	USoundCue* selfDestructSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FXs")
	USoundCue* exploadSound;

	bool isDead;

	FTimerHandle selfDamage_TimerHandle;
	float selfDamageInterval;
	bool startedSelfDistruction;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
