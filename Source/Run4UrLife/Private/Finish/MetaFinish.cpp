// Fill out your copyright notice in the Description page of Project Settings.


#include "Finish/MetaFinish.h"
#include "Run4UrLifeCharacter.h"
#include "Public/GameMode/MyGameModeBase.h"

// Sets default values
AMetaFinish::AMetaFinish()
{
	PrimaryActorTick.bCanEverTick = false;

	// Creamos la caja de colisión de la meta
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(200.0f, 100.0f, 100.0f)); // Una meta más ancha
	CollisionBox->SetCollisionProfileName(TEXT("Trigger"));

	bReplicates = true;
}

void AMetaFinish::BeginPlay()
{
	Super::BeginPlay();
    
	// Solo el servidor maneja quién llega a la meta para evitar hacks de los clientes
	if (HasAuthority())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AMetaFinish::OnOverlapBegin);
	}
}

void AMetaFinish::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
									UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
									bool bFromSweep, const FHitResult& SweepResult)
{
	// Verificamos si lo que cruzó es un jugador
	ARun4UrLifeCharacter* Personaje = Cast<ARun4UrLifeCharacter>(OtherActor);
    
	if (Personaje)
	{
		// Obtenemos el GameMode del servidor y le hacemos un Cast al tuyo
		if (AMiGameMode::StaticClass())
		{
			AMiGameMode::StaticClass();
		}
        
		AMiGameMode::StaticClass();
        
		if (AMiGameMode* GM = Cast<AMiGameMode>(GetWorld()->GetAuthGameMode()))
		{
			// Le pasamos el controlador del personaje que cruzó
			GM->JugadorLlegoAMeta(Personaje->GetController());
		}
	}
}

