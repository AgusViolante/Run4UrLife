// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "MetaFinish.generated.h"

UCLASS()
class RUN4URLIFE_API AMetaFinish : public AActor
{
	GENERATED_BODY()
	
public:	
	AMetaFinish();

protected:
	virtual void BeginPlay() override;

	// Caja de colisión para la línea de meta
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Meta")
	UBoxComponent* CollisionBox;

	// Función que se ejecuta al cruzar la meta
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						bool bFromSweep, const FHitResult& SweepResult);
};

/*public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};*/
