// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerUp.generated.h"

UCLASS()
class GEARSLIKESHOOTER_API ASPowerUp : public AActor
{
	GENERATED_BODY()
	
public:	
	ASPowerUp();

protected:
	UFUNCTION()
	void OnTickPowerUp();

	/* Time between powerup ticks */
	UPROPERTY(EditDefaultsOnly, Category="Power-ups")
	float powerUpInterval;
	/* Total time we apply the powerup effect */
	UPROPERTY(EditDefaultsOnly, Category = "Power-ups")
	int32 totalNumOfTicks;

	int32 ticksProcessed;
	FTimerHandle powerUpTick_TimerHandle;

	UPROPERTY(ReplicatedUsing = OnRep_PowerupActive)
	bool isPowerUpActive;
	UFUNCTION()
	void OnRep_PowerUpActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "Power-up")
	void OnPowerUpStateChanged(bool newIsActive);

public:
	void ActivatePowerUp();

	UFUNCTION(BlueprintImplementableEvent, Category="Power-up")
	void OnActivated();
	UFUNCTION(BlueprintImplementableEvent, Category = "Power-up")
	void OnPowerUpTicked();
	UFUNCTION(BlueprintImplementableEvent, Category = "Power-up")
	void OnExpired();
};
