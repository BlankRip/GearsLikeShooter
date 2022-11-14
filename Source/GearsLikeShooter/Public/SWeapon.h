// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> surfaceType;
	UPROPERTY()
	FVector_NetQuantize traceTo;
};

UCLASS()
class GEARSLIKESHOOTER_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	void StartFire();
	void StopFire();

protected:
	virtual void BeginPlay() override;
	virtual void Fire();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	void PlayImpactEffect(const FVector& impactPoint, const EPhysicalSurface& surfaceType);
	void ApplyDamage(const FHitResult& hit, const FVector& shotDirection, const EPhysicalSurface& surfaceType);
	void PlayFireEffects(const FVector& trailEndPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* meshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName muzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* muzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* defaultImpactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* fleshImpactEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName traileTargetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* trailEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> fireCamShakeClass;

	UPROPERTY(EditDefaultsOnly, Category="Gun Stats")
	float baseDamageAmt = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Gun Stats")
	TSubclassOf<UDamageType> damageType;

	FTimerHandle timerHandle_timeBtwShots;
	float lastShotTime;
	float timeBtwShots;
	UPROPERTY(EditDefaultsOnly, Category="Gun Stats")
	float rateOfFire; //Bullets per minute fired

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace hitScanTrace;
	UFUNCTION()
	void OnRep_HitScanTrace();
};
