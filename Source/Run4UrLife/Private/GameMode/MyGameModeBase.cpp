#include "Public/GameMode/MyGameModeBase.h"

#include "Engine/Engine.h"
#include "Public/GameMode/MyGameStateBase.h"
#include "Public/GameMode/MyPlayerState.h"

AMiGameMode::AMiGameMode()
{
	GameStateClass = AMiGameState::StaticClass();
	PlayerStateClass = AMiPlayerState::StaticClass();
}

void AMiGameMode::JugadorLlegoAMeta(AController* PlayerController)
{
	if (!PlayerController) return;

	AMiPlayerState* PS = Cast<AMiPlayerState>(PlayerController->PlayerState);
	AMiGameState* GS = GetGameState<AMiGameState>();

	if (PS && !PS->bHaLlegadoAMeta && GS)
	{
		// Marcamos que ya llegó para que no se repita
		PS->bHaLlegadoAMeta = true;

		// Se guardan los segundos
		PS->TiempoFinal = GetWorld()->GetTimeSeconds();

		// Se agrega al ranking
		GS->RankingClasificacion.Add(PS);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				FString::Printf(TEXT("¡%s llegó a la meta en %f segundos!"), *PS->GetPlayerName(), PS->TiempoFinal)
			);
		}
	}
}