// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USHealthComponent;
class ASWeapon;

UCLASS()
class GEARSLIKESHOOTER_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);

	void BeginCrouch();
	void EndCrouch();

	void EnterADS();
	void ExitADS();

	void StartFire();
	void EndFire();

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta, 
		const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* cameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* springArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* healthComp;

	UPROPERTY(Replicated)
	ASWeapon* currentWeapon;
	UPROPERTY(EditDefaultsOnly, Category="Player")
	TSubclassOf<ASWeapon> starterWeaponClass;
	UPROPERTY(VisibleAnywhere, Category="Player")
	FName weaponAttachSocketName;

	bool ads;
	UPROPERTY(EditDefaultsOnly, Category = "Player")
	float adsFOV;
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta=(ClampMin = 0.1f, ClampMax = 100.f))
	float adsInterpSpeed;
	float defaultFOV;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="Player")
	bool died;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
};
