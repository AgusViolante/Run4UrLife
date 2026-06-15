#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

class AController;

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
	
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> RaceHUDWidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Carrera")
	bool CantidadJugadores = false;

private:
	FTimerHandle TimerHandle_CuentaRegresiva;
};