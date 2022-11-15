// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent() {
	maxHealth = 100.f;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay() {
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* owner = GetOwner();
		if (owner) {
			owner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleTakeAnyDamage);
		}
	}
	health = maxHealth;
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, health);
}

void USHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f)
		return;
	health = FMath::Clamp(health - Damage, 0.f, maxHealth);

	OnHealthChanged.Broadcast(this, health, Damage, DamageType, InstigatedBy, DamageCauser);
}

void USHealthComponent::OnRep_Health(float oldHealth) {
	float _damage = health - oldHealth;
	OnHealthChanged.Broadcast(this, health, _damage, nullptr, nullptr, nullptr);
}
