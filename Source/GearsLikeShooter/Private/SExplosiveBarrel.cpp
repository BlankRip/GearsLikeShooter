// Fill out your copyright notice in the Description page of Project Settings.


#include "SExplosiveBarrel.h"
#include "Components/SHealthComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASExplosiveBarrel::ASExplosiveBarrel() {
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetSimulatePhysics(true);
	mesh->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = mesh;

	radialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("Radial Force Component"));
	radialForceComp->SetupAttachment(mesh);
	radialForceComp->Radius = 250.f;
	radialForceComp->bImpulseVelChange = true;
	radialForceComp->bAutoActivate = false;
	radialForceComp->bIgnoreOwningActor = true;

	healthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
	explosionImpulse = 400.f;
}

// Called when the game starts or when spawned
void ASExplosiveBarrel::BeginPlay() {
	Super::BeginPlay();
	
	healthComp->OnHealthChanged.AddDynamic(this, &ASExplosiveBarrel::OnHealthChanged);
}

void ASExplosiveBarrel::OnHealthChanged(USHealthComponent* PassedHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser)
{
	if (exploded)
		return;
	if (Health <= 0) {
		exploded = true;

		FVector boostIntensity = FVector::UpVector * explosionImpulse;
		mesh->AddImpulse(boostIntensity, NAME_None, true);

		if (explosionEffect)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorTransform());
		if (explodedMaterial)
			mesh->SetMaterial(0, explodedMaterial);

		radialForceComp->FireImpulse();
	}
}