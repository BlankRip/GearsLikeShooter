// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

static int32 debugWeaponDrawing = 0;
FAutoConsoleVariableRef CVar_DebugWeapon(
	TEXT("COOP.DebugWeapons"),
	debugWeaponDrawing,
	TEXT("Draw Debug Lines for Weapons"),
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = meshComp;

	muzzleSocketName = "MuzzleSocket";
	traileTargetName = "Target";
}

void ASWeapon::Fire() {
	AActor* owner = GetOwner();

	if (owner) {
		FVector eyeLocation;
		FRotator eyeRotation;
		owner->GetActorEyesViewPoint(eyeLocation, eyeRotation);
		FVector shotDirection = eyeRotation.Vector();
		FVector traceEndLocation = eyeLocation + (shotDirection * 10000.f);
		FVector trailEndPoint = traceEndLocation;
		
		FCollisionQueryParams querryParams;
		querryParams.AddIgnoredActor(owner);
		querryParams.AddIgnoredActor(this);
		querryParams.bTraceComplex = true;
		FHitResult hit;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndLocation, ECC_Visibility, querryParams)) {
			AActor* hitActor = hit.GetActor();

			UGameplayStatics::ApplyPointDamage(hitActor, damageAmt, shotDirection, hit, owner->GetInstigatorController(), this, damageType);
			trailEndPoint = hit.ImpactPoint;
			if(impactEffect)
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), impactEffect, hit.ImpactPoint, hit.ImpactNormal.Rotation());
		}
		PlayFireEffects(trailEndPoint);

		if(debugWeaponDrawing > 0)
			DrawDebugLine(GetWorld(), eyeLocation, traceEndLocation, FColor::White, false, 1.f, 0, 1.f);
	}
}

void ASWeapon::PlayFireEffects(const FVector& trailEndPoint) {
	if (muzzleEffect)
		UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);

	if (trailEffect) {
		FVector muzzleLoction = meshComp->GetSocketLocation(muzzleSocketName);
		UParticleSystemComponent* trailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), trailEffect, muzzleLoction);
		if (trailComp) {
			trailComp->SetVectorParameter(traileTargetName, trailEndPoint);
		}
	}
}
