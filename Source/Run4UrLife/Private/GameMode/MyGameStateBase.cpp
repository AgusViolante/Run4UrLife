#include "Public/GameMode/MyGameStateBase.h"

#include "Net/UnrealNetwork.h"

AMiGameState::AMiGameState()
{
	bReplicates = true;
}

void AMiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Se replica el ranking
	DOREPLIFETIME(AMiGameState, RankingClasificacion);
}