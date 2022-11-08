// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UParticleSystem;
//class UCameraShake;

UCLASS()
class GEARSLIKESHOOTER_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

	UFUNCTION(BlueprintCallable)
	virtual void Fire();

protected:
	void PlayFireEffects(const FVector& trailEndPoint);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* meshComp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName muzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* muzzleEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* impactEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	FName traileTargetName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* trailEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TSubclassOf<UCameraShakeBase> fireCamShakeClass;

	UPROPERTY(EditDefaultsOnly, Category="Gun Stats")
	float damageAmt = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GunStats")
	TSubclassOf<UDamageType> damageType;
};
