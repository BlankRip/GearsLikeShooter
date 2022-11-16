// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerUp.h"

ASPowerUp::ASPowerUp() {
	powerUpInterval = 0.f;
	totalNumOfTicks = 0.f;
}

void ASPowerUp::BeginPlay() {
	Super::BeginPlay();

}

void ASPowerUp::OnTickPowerUp() {
	ticksProcessed++;
	OnPowerUpTicked();
	if (ticksProcessed >= totalNumOfTicks) {
		OnExpired();
		GetWorldTimerManager().ClearTimer(powerUpTick_TimerHandle);
	}
}

void ASPowerUp::ActivatePowerUp() {
	OnActivated();
	if (powerUpInterval > 0.f) {
		GetWorldTimerManager().SetTimer(powerUpTick_TimerHandle, this, &ASPowerUp::OnTickPowerUp,
			powerUpInterval, true);
	} else {
		OnTickPowerUp();
	}
}
