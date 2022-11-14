// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GearsLikeShooter/GearsLikeShooter.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

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
	rateOfFire = 600.f;

	SetReplicates(true);
	NetUpdateFrequency = 66.f;
	MinNetUpdateFrequency = 33.f;
}

void ASWeapon::BeginPlay() {
	Super::BeginPlay();

	timeBtwShots = 60/rateOfFire;
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, hitScanTrace, COND_SkipOwner);
}

void ASWeapon::StartFire() {
	float firstDely = FMath::Max(lastShotTime + timeBtwShots - GetWorld()->TimeSeconds, 0.f);
	GetWorldTimerManager().SetTimer(timerHandle_timeBtwShots, this, &ASWeapon::Fire, timeBtwShots, true, firstDely);
}

void ASWeapon::StopFire() {
	GetWorldTimerManager().ClearTimer(timerHandle_timeBtwShots);
}

void ASWeapon::Fire() {
	if (GetLocalRole() < ROLE_Authority) {
		ServerFire();
	}

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
		querryParams.bReturnPhysicalMaterial = true;
		FHitResult hit;
		EPhysicalSurface surfaceType = EPhysicalSurface::SurfaceType_Default;
		if (GetWorld()->LineTraceSingleByChannel(hit, eyeLocation, traceEndLocation, Collision_Weapon, querryParams)) {

			surfaceType = UPhysicalMaterial::DetermineSurfaceType(hit.PhysMaterial.Get());
			ApplyDamage(hit, shotDirection, surfaceType);
			PlayImpactEffect(hit.ImpactPoint, surfaceType);
			trailEndPoint = hit.ImpactPoint;
		}
		PlayFireEffects(trailEndPoint);
		if (GetLocalRole() == ROLE_Authority) {
			hitScanTrace.traceTo = trailEndPoint;
			hitScanTrace.surfaceType = surfaceType;
		}
		lastShotTime = GetWorld()->TimeSeconds;

		if(debugWeaponDrawing > 0)
			DrawDebugLine(GetWorld(), eyeLocation, traceEndLocation, FColor::White, false, 1.f, 0, 1.f);
	}
}

void ASWeapon::OnRep_HitScanTrace() {
	PlayFireEffects(hitScanTrace.traceTo);
	PlayImpactEffect(hitScanTrace.traceTo, hitScanTrace.surfaceType);
}

void ASWeapon::ServerFire_Implementation() {
	Fire();
}

bool ASWeapon::ServerFire_Validate() {
	return true;
}

void ASWeapon::ApplyDamage(const FHitResult& hit, const FVector& shotDirection, const EPhysicalSurface& surfaceType) {
	AActor* hitActor = hit.GetActor();
	if (hitActor) {
		float currentDamage = baseDamageAmt;
		if (surfaceType == Surface_FleshVulnerable) {
			currentDamage *= 3.f;
		}
		UGameplayStatics::ApplyPointDamage(hitActor, currentDamage, shotDirection, hit, GetOwner()->GetInstigatorController(), this, damageType);
	}
}

void ASWeapon::PlayImpactEffect(const FVector& impactPoint, const EPhysicalSurface& surfaceType) {
	UParticleSystem* selectedEffect = nullptr;
	switch (surfaceType) {
	/*case EPhysicalSurface::SurfaceType_Default:
		break;*/
	case Surface_FleshDefault:
		selectedEffect = fleshImpactEffect;
		break;
	case Surface_FleshVulnerable:
		selectedEffect = fleshImpactEffect;
		break;
	default:
		selectedEffect = defaultImpactEffect;
		break;
	}
	if (selectedEffect) {
		FVector muzzleLoction = meshComp->GetSocketLocation(muzzleSocketName);
		FVector impactDirection = impactPoint - muzzleLoction;
		impactDirection.Normalize();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), selectedEffect, impactPoint, impactDirection.Rotation());
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

	APawn* owner = Cast<APawn>(GetOwner());
	if (owner) {
		APlayerController* playerController = Cast<APlayerController>(owner->GetController());
		if (playerController)
			playerController->ClientPlayCameraShake(fireCamShakeClass);
	}
}
