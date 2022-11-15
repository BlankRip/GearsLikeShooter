// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class GEARSLIKESHOOTER_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetNextPathPoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Movement", meta = (AllowPrivateAccess = "true"))
	float movementForce;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Movement", meta = (AllowPrivateAccess = "true"))
	bool useVelocityChange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tracker Movement", meta = (AllowPrivateAccess = "true"))
	float requiredDistanceToTarget;
	FVector nextPathPoint;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};