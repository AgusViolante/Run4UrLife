// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlataformaMovil.generated.h"

class UStaticMeshComponent;

UCLASS()
class RUN4URLIFE_API APlataformaMovil : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlataformaMovil();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, Category = "Componentes")
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	
	FVector UbicacionInicial;
    
	UPROPERTY(EditAnywhere, Category = "Movimiento")
	FVector DireccionMovimiento = FVector(1.f, 0.f, 0.f); 

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float Velocidad = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movimiento")
	float DistanciaMaxima = 500.f;

};
