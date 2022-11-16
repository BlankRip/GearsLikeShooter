// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp.h"
#include "Net/UnrealNetwork.h"

ASPowerUp::ASPowerUp() {
	powerUpInterval = 0.f;
	totalNumOfTicks = 0.f;
	isPowerUpActive = false;
	SetReplicates(true);
}

void ASPowerUp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUp, isPowerUpActive);
}

void ASPowerUp::OnTickPowerUp() {
	ticksProcessed++;
	OnPowerUpTicked();
	if (ticksProcessed >= totalNumOfTicks) {
		OnExpired();
		isPowerUpActive = false;
		OnRep_PowerUpActive();
		GetWorldTimerManager().ClearTimer(powerUpTick_TimerHandle);
	}
}

void ASPowerUp::OnRep_PowerUpActive() {
	OnPowerUpStateChanged(isPowerUpActive);
}

void ASPowerUp::ActivatePowerUp() {
	OnActivated();
	isPowerUpActive = true;
	OnRep_PowerUpActive();
	if (powerUpInterval > 0.f) {
		GetWorldTimerManager().SetTimer(powerUpTick_TimerHandle, this, &ASPowerUp::OnTickPowerUp,
			powerUpInterval, true);
	} else {
		OnTickPowerUp();
	}
}
