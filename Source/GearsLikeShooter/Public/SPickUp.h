// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickUp.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerUp;

UCLASS()
class GEARSLIKESHOOTER_API ASPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void Respawn();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USphereComponent* sphereComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UDecalComponent* decalComp;

	UPROPERTY(EditAnywhere, Category="Power-up Actor")
	TSubclassOf<ASPowerUp> powerUpClass;
	ASPowerUp* spawnedPowerUp;
	UPROPERTY(EditAnywhere, Category = "Power-up Actor")
	float cooldownDuration;
	FTimerHandle respawn_TimerHandle;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
