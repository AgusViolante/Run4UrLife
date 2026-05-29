#include "Public/GameMode/MyGameStateBase.h"
#include "Net/UnrealNetwork.h"

AMiGameState::AMiGameState()
{
	bool bReplicated = true;
	TiempoCuentaRegresiva = 10;
	bCarreraIniciada = false;
	bCarreraTerminada = false;
}

void AMiGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMiGameState, RankingClasificacion);
	DOREPLIFETIME(AMiGameState, TiempoCuentaRegresiva);
	DOREPLIFETIME(AMiGameState, bCarreraIniciada);
	DOREPLIFETIME(AMiGameState, bCarreraTerminada);
}