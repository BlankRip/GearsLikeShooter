// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent() {
	maxHealth = 100.f;
}


// Called when the game starts
void USHealthComponent::BeginPlay() {
	Super::BeginPlay();

	AActor* owner = GetOwner();
	if (owner) {
		owner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
	}
	health = maxHealth;
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f)
		return;
	health = FMath::Clamp(health - Damage, 0.f, maxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Health Changed to: %f"), health);

	OnHealthChanged.Broadcast(this, health, Damage, DamageType, InstigatedBy, DamageCauser);
}
