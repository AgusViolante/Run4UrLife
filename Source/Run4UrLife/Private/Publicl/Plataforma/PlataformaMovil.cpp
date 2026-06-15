// Fill out your copyright notice in the Description page of Project Settings.


#include "Publicl/Plataforma/PlataformaMovil.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APlataformaMovil::APlataformaMovil()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshPlataforma"));
	RootComponent = MeshComponent;

	MeshComponent->SetMobility(EComponentMobility::Movable);

	bReplicates = true;
	
	SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void APlataformaMovil::BeginPlay()
{
	Super::BeginPlay();

	UbicacionInicial = GetActorLocation();

	DireccionMovimiento.Normalize();
	
}

// Called every frame
void APlataformaMovil::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector UbicacionActual = GetActorLocation();
	FVector NuevaUbicacion = UbicacionActual + (DireccionMovimiento * Velocidad * DeltaTime);

	SetActorLocation(NuevaUbicacion);

	float DistanciaRecorrida = FVector::Distance(UbicacionInicial, NuevaUbicacion);

	if (DistanciaRecorrida >= DistanciaMaxima)
	{

		DireccionMovimiento = -DireccionMovimiento;
        
		UbicacionInicial = NuevaUbicacion;
	}

}

