// Fill out your copyright notice in the Description page of Project Settings.


#include "Public/GameMode/MyPlayerState.h"
#include "Net/UnrealNetwork.h"

AMiPlayerState::AMiPlayerState()
{
	UltimoCheckpoint = FVector::ZeroVector; 
	TiempoFinal = 0.0f; 
	bHaLlegadoAMeta = false; 

	bReplicates = true;
}

void AMiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMiPlayerState, UltimoCheckpoint);
	DOREPLIFETIME(AMiPlayerState, TiempoFinal);
	DOREPLIFETIME(AMiPlayerState, bHaLlegadoAMeta);
}