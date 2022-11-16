// Fill out your copyright notice in the Description page of Project Settings.


#include "SPickUp.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
ASPickUp::ASPickUp() {
	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetSphereRadius(75.f);
	RootComponent = sphereComp;

	decalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	decalComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	decalComp->DecalSize = FVector(64.f, 75.f, 75.f);
	decalComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASPickUp::BeginPlay() {
	Super::BeginPlay();
	
}

void ASPickUp::NotifyActorBeginOverlap(AActor* OtherActor) {
	Super::NotifyActorBeginOverlap(OtherActor);
}

