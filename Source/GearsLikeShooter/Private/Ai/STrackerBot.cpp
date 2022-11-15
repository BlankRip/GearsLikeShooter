// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Components/SHealthComponent.h"
#include "Components/SphereComponent.h"
#include "SCharacter.h"

// Sets default values
ASTrackerBot::ASTrackerBot() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCanEverAffectNavigation(false);
	mesh->SetSimulatePhysics(true);
	RootComponent = mesh;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Shpere Component"));
	sphereComp->SetSphereRadius(200.f);
	sphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	sphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	sphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	sphereComp->SetupAttachment(RootComponent);

	healthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));
	healthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::HandleTakeDamage);

	movementForce = 1000.f;
	requiredDistanceToTarget = 100.f;

	explosionRadius = 200.f;
	explostionDamage = 40.f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay() {
	Super::BeginPlay();
	
	nextPathPoint = GetNextPathPoint();
}

FVector ASTrackerBot::GetNextPathPoint() {
	ACharacter* _playerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);
	UNavigationPath* _newPath = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetActorLocation(), _playerPawn);
	if (_newPath->PathPoints.Num() > 1)
		return _newPath->PathPoints[1];

	return GetActorLocation();
}

void ASTrackerBot::SelfDestruct() {
	if (isDead)
		return;

	isDead = true;
	if(explosionEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), explosionEffect, GetActorLocation());
	TArray<AActor*> ignoredActors;
	ignoredActors.Add(this);
	UGameplayStatics::ApplyRadialDamage(this, explostionDamage, GetActorLocation(), explosionRadius, 
		nullptr, ignoredActors, this, GetInstigatorController(), true);
	Destroy();
}

void ASTrackerBot::DamageSelf() {
	UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
}

void ASTrackerBot::HandleTakeDamage(USHealthComponent* PassedHealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser)
{
	if(matInst == nullptr)
		matInst = mesh->CreateDynamicMaterialInstance(0, mesh->GetMaterial(0));
	if(matInst)
		matInst->SetScalarParameterValue("LastTimeDmgTaken", GetWorld()->TimeSeconds);
	if (Health <= 0)
		SelfDestruct();
	UE_LOG(LogTemp, Log, TEXT("Health %f of %s"), Health, *GetName());
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float _distanceToTarget = (GetActorLocation() - nextPathPoint).Size();
	if (_distanceToTarget > requiredDistanceToTarget) {
		FVector _forceDir = nextPathPoint - GetActorLocation();
		_forceDir.Normalize();
		_forceDir *= movementForce;
		mesh->AddForce(_forceDir, NAME_None, useVelocityChange);
	} else {
		nextPathPoint = GetNextPathPoint();
	}
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor) {
	if (startedSelfDistruction)
		return;

	ASCharacter* playerPawn = Cast<ASCharacter>(OtherActor);
	if (playerPawn) {
		GetWorldTimerManager().SetTimer(selfDamage_TimerHandle, this, &ASTrackerBot::DamageSelf, 0.5f, true, 0.f);
		startedSelfDistruction = true;
	}
}

