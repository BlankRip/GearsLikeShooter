// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileWeapon.h"

void ASProjectileWeapon::Fire() {
	AActor* owner = GetOwner();

	if (owner && projectileClass) {
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);

		FVector muzzleLocation = meshComp->GetSocketLocation(muzzleSocketName);
		FRotator muzzleRotation = meshComp->GetSocketRotation(muzzleSocketName);
		FActorSpawnParameters spawnParmas;
		spawnParmas.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AActor>(projectileClass, muzzleLocation, eyeRotation, spawnParmas);
	}
}
