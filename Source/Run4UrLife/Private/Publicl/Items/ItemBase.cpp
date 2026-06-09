// Fill out your copyright notice in the Description page of Project Settings.


#include "Publicl/Items/ItemBase.h"
#include "Interfaces/RecogiblesInterface.h"
#include "Run4UrLifeCharacter.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true; 
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(100.0f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::ItemOverlap);
	
	ItemState = EItemState::Empty;

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority() && SphereComponent != nullptr)
	{
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemBase::ItemOverlap);
	}
}

void AItemBase::ItemOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority() && OtherActor != nullptr)
	{
		IRecogiblesInterface* InterfazAgarrar = Cast<IRecogiblesInterface>(OtherActor);

		if (InterfazAgarrar != nullptr)
		{
			InterfazAgarrar->RecibirObjeto(ItemState, CantidadAOtorgar);

			Destroy(); 
		}
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

