#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

UCLASS()
class RUN4URLIFE_API AMiGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMiGameMode();

	UFUNCTION(BlueprintCallable, Category = "Carrera")
	void JugadorLlegoAMeta(AController* PlayerController);

protected:
	virtual void BeginPlay() override;
	
	void ActualizarCuentaRegresiva();
	
	void IniciarCarrera();
	
	void FinalizarCarrera();
	
	virtual void OnPostLogin(AController* NewPlayer) override;

private:
	FTimerHandle TimerHandle_CuentaRegresiva;
};