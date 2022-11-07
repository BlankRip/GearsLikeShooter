// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	meshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = meshComp;

	muzzleSocketName = "MuzzleSocket";
	traileTargetName = "Target";
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
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
		if(muzzleEffect)
			UGameplayStatics::SpawnEmitterAttached(muzzleEffect, meshComp, muzzleSocketName);

		if (trailEffect) {
			FVector muzzleLoction = meshComp->GetSocketLocation(muzzleSocketName);
			UParticleSystemComponent* trailComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), trailEffect, muzzleLoction);
			if (trailComp) {
				trailComp->SetVectorParameter(traileTargetName, trailEndPoint);
			}
		}

		//DrawDebugLine(GetWorld(), eyeLocation, traceEndLocation, FColor::White, false, 1.f, 0, 1.f);
	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

