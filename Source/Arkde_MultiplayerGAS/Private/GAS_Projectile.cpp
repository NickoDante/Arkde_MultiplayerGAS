// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Sets default values
AGAS_Projectile::AGAS_Projectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Called when the game starts or when spawned
void AGAS_Projectile::BeginPlay()
{
	Super::BeginPlay();
	
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Called every frame
void AGAS_Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AGAS_Projectile::Multicast_IgnoreActor_Implementation(AActor* ActorToIgnore)
{
	SphereComponent->IgnoreActorWhenMoving(ActorToIgnore, true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

bool AGAS_Projectile::Multicast_IgnoreActor_Validate(AActor* ActorToIgnore)
{
	return true;
}