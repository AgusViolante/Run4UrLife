#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"


UCLASS()
class RUN4URLIFE_API AMiGameState : public AGameStateBase
{
    GENERATED_BODY()
    
public:
    AMiGameState();

    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Carrera")
    TArray<APlayerState*> RankingClasificacion;
    
    //cuenta regresiva
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Carrera")
    int32 TiempoCuentaRegresiva;

    // B Carrera activa o no
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Carrera")
    bool bCarreraIniciada;

    // B Carrera termino
    UPROPERTY(BlueprintReadOnly, Replicated, Category = "Carrera")
    bool bCarreraTerminada;

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};