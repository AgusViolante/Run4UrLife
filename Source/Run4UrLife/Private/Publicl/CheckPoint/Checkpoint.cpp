// Fill out your copyright notice in the Description page of Project Settings.

#include "Publicl/CheckPoint/Checkpoint.h"
#include "Public/GameMode/MyPlayerState.h"
#include "Run4UrLifeCharacter.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent ->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	BoxComponent ->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent ->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent ->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComponent = BoxComponent;

}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::CheckOverlap);
	}
	
}

void ACheckpoint::CheckOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARun4UrLifeCharacter* Personaje = Cast<ARun4UrLifeCharacter>(OtherActor))
	{
		if (AMiPlayerState* PS = Cast<AMiPlayerState>(Personaje->GetPlayerState()))
		{
			
			PS->UltimoCheckpoint = GetActorLocation();
            
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("¡Checkpoint Guardado!"));
		}
	}
}

// Called every frame
void ACheckpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

