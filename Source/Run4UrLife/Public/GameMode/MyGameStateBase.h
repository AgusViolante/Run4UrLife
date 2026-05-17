#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "MyGameStateBase.generated.h"

UCLASS()
class RUN4URLIFE_API AMiGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMiGameState();

	// Se crea un Array para ver los jugadores
	UPROPERTY(BlueprintReadOnly, Replicated, Category = "Carrera")
	TArray<APlayerState*> RankingClasificacion;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};