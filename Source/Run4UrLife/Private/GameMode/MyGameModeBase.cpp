#include "Public/GameMode/MyGameModeBase.h"

#include "Engine/Engine.h"
#include "Public/GameMode/MyGameStateBase.h"
#include "Public/GameMode/MyPlayerState.h"
#include "Run4UrLifeCharacter.h"
#include "TimerManager.h"

AMiGameMode::AMiGameMode()
{
    GameStateClass = AMiGameState::StaticClass();
    PlayerStateClass = AMiPlayerState::StaticClass();
}

void AMiGameMode::BeginPlay()
{
    Super::BeginPlay();

    // empieza el timer para que arranque la carrera
    GetWorldTimerManager().SetTimer(TimerHandle_CuentaRegresiva, this, &AMiGameMode::ActualizarCuentaRegresiva, 1.0f, true);
}

void AMiGameMode::ActualizarCuentaRegresiva()
{
    AMiGameState* GS = GetGameState<AMiGameState>();
    if (!GS) return;

    if (GS->TiempoCuentaRegresiva > 1)
    {
        GS->TiempoCuentaRegresiva--;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("Empieza en: %d"), GS->TiempoCuentaRegresiva));
    }
    else
    {
        // arranca la carrera cuando termina el timer
        GetWorldTimerManager().ClearTimer(TimerHandle_CuentaRegresiva);
        IniciarCarrera();
    }
}

void AMiGameMode::IniciarCarrera()
{
    AMiGameState* GS = GetGameState<AMiGameState>();
    if (GS)
    {
        GS->bCarreraIniciada = true;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("¡¡¡YA!!!"));
        
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            APlayerController* PC = It->Get();
            if (PC)
            {
                if (ARun4UrLifeCharacter* Personaje = Cast<ARun4UrLifeCharacter>(PC->GetPawn()))
                {
                    Personaje->ActivarMovimiento(); // ¡A correr!
                }
            }
        }
    }
}

void AMiGameMode::OnPostLogin(APlayerController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);

    // Si la carrera todavía NO empezó, congelamos al jugador que acaba de entrar
    AMiGameState* GS = GetGameState<AMiGameState>();
    if (GS && !GS->bCarreraIniciada)
    {
        // Esperamos un pequeño frame a que el personaje aparezca (Pawn)
        if (APawn* PlayerPawn = NewPlayer->GetPawn())
        {
            if (ARun4UrLifeCharacter* Personaje = Cast<ARun4UrLifeCharacter>(PlayerPawn))
            {
                Personaje->DesactivarMovimiento();
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Jugador conectado: Congelado esperando el inicio."));
            }
        }
    }
}

void AMiGameMode::JugadorLlegoAMeta(AController* PlayerController)
{
  
    if (!PlayerController) return;

 
    AMiPlayerState* PS = Cast<AMiPlayerState>(PlayerController->PlayerState);
    AMiGameState* GS = GetGameState<AMiGameState>();

    
    if (PS && !PS->bHaLlegadoAMeta && GS)
    {
       
        PS->bHaLlegadoAMeta = true;
        

        float TiempoDeCarrera = GetWorld()->GetTimeSeconds() - 5.0f;
        
      
        PS->TiempoFinal = TiempoDeCarrera;

   
        GS->RankingClasificacion.Add(PS);

   
        int32 PosicionOcupada = GS->RankingClasificacion.Num();

    
        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1, 
                6.f, 
                FColor::Green, 
                FString::Printf(TEXT("¡%s cruzó la META! Puesto: #%d | Tiempo: %f segundos"), *PS->GetPlayerName(), PosicionOcupada, TiempoDeCarrera)
            );
        }

 
        int32 TotalJugadores = GetNumPlayers();

       
        if (GS->RankingClasificacion.Num() == TotalJugadores)
        {
            FinalizarCarrera();
        }
    }
}

void AMiGameMode::FinalizarCarrera()
{
    AMiGameState* GS = GetGameState<AMiGameState>();
    if (GS)
    {
        GS->bCarreraTerminada = true;
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, TEXT("CARRERA FINALIZADA. Mostrando Podio..."));
        
    }
}