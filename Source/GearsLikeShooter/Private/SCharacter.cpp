// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GearsLikeShooter/GearsLikeShooter.h"
#include "Components/SHealthComponent.h"
#include "SWeapon.h"

// Sets default values
ASCharacter::ASCharacter() {
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	springArmComp->bUsePawnControlRotation = true;
	springArmComp->SetupAttachment(RootComponent);

	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	cameraComp->SetupAttachment(springArmComp);

	healthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health Component"));

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(Collision_Weapon, ECR_Ignore);

	adsFOV = 65.f;
	adsInterpSpeed = 20.f;
	weaponAttachSocketName = "WeaponSocket";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay() {
	Super::BeginPlay();
	
	defaultFOV = cameraComp->FieldOfView;

	if (starterWeaponClass) {
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		currentWeapon = GetWorld()->SpawnActor<ASWeapon>(starterWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);
		if (currentWeapon) {
			currentWeapon->SetOwner(this);
			currentWeapon->AttachToComponent(GetMesh(), 
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				weaponAttachSocketName);
		}
	}

	healthComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float value) {
	AddMovementInput(GetActorForwardVector() * value);
}

void ASCharacter::MoveRight(float value) {
	AddMovementInput(GetActorRightVector() * value);
}

void ASCharacter::BeginCrouch() {
	Crouch();
}

void ASCharacter::EndCrouch() {
	UnCrouch();
}

void ASCharacter::EnterADS() {
	ads = true;
}

void ASCharacter::ExitADS() {
	ads = false;
}

void ASCharacter::StartFire() {
	if (currentWeapon)
		currentWeapon->StartFire();
}

void ASCharacter::EndFire() {
	if (currentWeapon)
		currentWeapon->StopFire();
}

void ASCharacter::OnHealthChanged(USHealthComponent* HealthComp, float Health, float HealthDelta,
	const class UDamageType* DamageType, class AController* InstigatedBy, class AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Health Changed to: %f"), Health);
	if (!died && Health <= 0.f) {
		died = true;

		GetMovementComponent()->StopMovementImmediately();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		DetachFromControllerPendingDestroy();
		SetLifeSpan(10.f);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	float targetFOV = ads ? adsFOV : defaultFOV;
	float newFOV = FMath::FInterpTo(cameraComp->FieldOfView, targetFOV, DeltaTime, adsInterpSpeed);
	cameraComp->SetFieldOfView(newFOV);
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &ASCharacter::EnterADS);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &ASCharacter::ExitADS);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &ASCharacter::StartFire);
	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &ASCharacter::EndFire);
}

FVector ASCharacter::GetPawnViewLocation() const
{
	if (cameraComp)
		return cameraComp->GetComponentLocation();
	
	return Super::GetPawnViewLocation();
}