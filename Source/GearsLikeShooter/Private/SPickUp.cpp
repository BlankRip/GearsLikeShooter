// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUp.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "SPowerUp.h"
#include "TimerManager.h"

// Sets default values
ASPickUp::ASPickUp() {
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetSphereRadius(75.f);
	RootComponent = sphereComp;

	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	decalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	decalComp->DecalSize = FVector(64.f, 75.f, 75.f);
	decalComp->SetupAttachment(RootComponent);

	cooldownDuration = 10.f;
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASPickUp::BeginPlay() {
	Super::BeginPlay();
	
	if(GetLocalRole() == ROLE_Authority)
		Respawn();
}

void ASPickUp::Respawn() {
	if (powerUpClass == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Power up call is set to null in %s, update the blueprint"), *GetName());
		return;
	}

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	spawnedPowerUp = GetWorld()->SpawnActor<ASPowerUp>(powerUpClass, GetTransform(), spawnParams);
}

void ASPickUp::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);

	if (GetLocalRole() == ROLE_Authority && spawnedPowerUp) {
		spawnedPowerUp->ActivatePowerUp();
		spawnedPowerUp = nullptr;

		GetWorldTimerManager().SetTimer(respawn_TimerHandle, this, &ASPickUp::Respawn, cooldownDuration);
	}
}

