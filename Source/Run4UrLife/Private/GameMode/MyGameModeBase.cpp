#include "Public/GameMode/MyGameModeBase.h"

#include "Engine/Engine.h"
#include "Public/GameMode/MyGameStateBase.h"
#include "Public/GameMode/MyPlayerState.h"
#include "Run4UrLifeCharacter.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

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
    if (GetNumPlayers() < 2) //Esperamos que haya 2 jugadores
    {
        if (GEngine) 
        {
            GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Esperando jugadores... (Mínimo 2 para iniciar)"));
        }
        return; 
    }
    
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
    /*AMiGameState* GS = GetGameState<AMiGameState>();
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
                    Personaje->ActivarMovimiento(); 
                }
            }
        }
    }*/
    AMiGameState* GS = GetGameState<AMiGameState>();
    if (GS)
    {
        GS->bCarreraIniciada = true;
        
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("¡¡¡YA!!!"));
    }
}

void AMiGameMode::OnPostLogin(AController* NewPlayer)
{
    Super::OnPostLogin(NewPlayer);

    AMiGameState* GS = GetGameState<AMiGameState>();
    if (GS && !GS->bCarreraIniciada)
    {
        if (APawn* PlayerPawn = NewPlayer->GetPawn())
        {
            if (ARun4UrLifeCharacter* Personaje = Cast<ARun4UrLifeCharacter>(PlayerPawn))
            {
                Personaje->DesactivarMovimiento();
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Jugador conectado: Congelado esperando el inicio."));
            }
        }
        if (GetNumPlayers() >= 2)
        {
            if (!GetWorldTimerManager().IsTimerActive(TimerHandle_CuentaRegresiva))
            {
                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("¡Mínimo de jugadores alcanzado! Iniciando cuenta regresiva..."));
                
                GetWorldTimerManager().SetTimer(TimerHandle_CuentaRegresiva, this, &AMiGameMode::ActualizarCuentaRegresiva, 1.0f, true);
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
    if (AMiGameState* GS = GetGameState<AMiGameState>())
    {
        GS->bCarreraIniciada = false;
        
        TArray<AActor*> PuntosPodio1;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Podio1"), PuntosPodio1);

        AActor* SpotGanador = (PuntosPodio1.Num() > 0) ? PuntosPodio1[0] : nullptr;

        // tepea solo al indice 0
        if (SpotGanador && GS->RankingClasificacion.Num() > 0)
        {
            TArray<APlayerState*>::ElementType PS_Ganador = GS->RankingClasificacion[0];
            if (PS_Ganador)
            {
                APawn* PawnGanador = PS_Ganador->GetPawn();
                if (PawnGanador)
                {
                    FVector UbicacionDestino = SpotGanador->GetActorLocation();
                    FRotator RotacionDestino = SpotGanador->GetActorRotation();

                    // tp al ganador al cubo
                    PawnGanador->SetActorLocationAndRotation(UbicacionDestino, RotacionDestino, false, nullptr, ETeleportType::TeleportPhysics);
                }
            }
        }

        // Fija las camaras
        if (SpotGanador)
        {
            for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
            {
                if (APlayerController* PC = It->Get())
                {
                    if (ARun4UrLifeCharacter* Char = Cast<ARun4UrLifeCharacter>(PC->GetPawn()))
                    {
                        //Enfoca al ganador
                        Char->Client_EnfocarGanador(SpotGanador);
                    }
                }
            }
        }
    }
}