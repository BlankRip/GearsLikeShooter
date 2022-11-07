// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class GEARSLIKESHOOTER_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void Fire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	USkeletalMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, Category="Gun Stats")
	float damageAmt = 20.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="GunStats")
	TSubclassOf<UDamageType> damageType;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
