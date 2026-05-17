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
};