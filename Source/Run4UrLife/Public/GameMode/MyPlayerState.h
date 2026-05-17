#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

UCLASS()
class RUN4URLIFE_API AMiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AMiPlayerState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Carrera")
	FVector UltimoCheckpoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Carrera")
	float TiempoFinal;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated, Category = "Carrera")
	bool bHaLlegadoAMeta;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};