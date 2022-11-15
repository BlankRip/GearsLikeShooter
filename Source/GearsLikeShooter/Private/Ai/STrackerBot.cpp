// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"

// Sets default values
ASTrackerBot::ASTrackerBot() {
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mesh->SetCanEverAffectNavigation(false);
	mesh->SetSimulatePhysics(true);
	RootComponent = mesh;

	movementForce = 1000.f;
	requiredDistanceToTarget = 100.f;
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
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

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
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

