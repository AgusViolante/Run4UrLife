// Copyright Epic Games, Inc. All Rights Reserved.

#include "Run4UrLifeCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/GameMode/MyPlayerState.h"
#include "GameFramework/PlayerController.h"
#include "Public/Publicl/Items/ItemBase.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Run4UrLife.h"
#include "GameMode/MyGameStateBase.h"
#include "Kismet/GameplayStatics.h"

ARun4UrLifeCharacter::ARun4UrLifeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ARun4UrLifeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARun4UrLifeCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ARun4UrLifeCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARun4UrLifeCharacter::Look);
		
		EnhancedInputComponent->BindAction(UsarItemAction, ETriggerEvent::Started, this, &ARun4UrLifeCharacter::UsarItemEquipado);
	}
	else
	{
		UE_LOG(LogRun4UrLife, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARun4UrLifeCharacter::Server_UsarItemEquipado_Implementation()
{
	if (ItemEquipado == EItemState::Empty) return;
	
	if (ItemEquipado == EItemState::Velocity)
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->MaxWalkSpeed = 1200.f; // Suma velocidad
            
			// Cdura tres segundos
			FTimerHandle TimerVelocidad;
			GetWorldTimerManager().SetTimer(TimerVelocidad, [this]()
			{
				if (GetCharacterMovement()) GetCharacterMovement()->MaxWalkSpeed = 600.f; // vuevle a 600
			}, 3.0f, false);
		}
	}
	else if (ItemEquipado == EItemState::Tramp)
	{
		
		TArray<AActor*> ListaPersonajes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARun4UrLifeCharacter::StaticClass(), ListaPersonajes);

		for (AActor* ActorRival : ListaPersonajes)
		{
			
			ARun4UrLifeCharacter* Rival = Cast<ARun4UrLifeCharacter>(ActorRival);
			if (Rival && Rival != this)
			{
				
				if (Rival->GetCharacterMovement())
				{
					Rival->GetCharacterMovement()->MaxWalkSpeed = 150.f; // velocidad lenta

					
					FTimerHandle TimerRalentizacion;
					GetWorldTimerManager().SetTimer(TimerRalentizacion, [Rival]()
					{
						if (Rival && Rival->GetCharacterMovement())
						{
							Rival->GetCharacterMovement()->MaxWalkSpeed = 600.f; // velocdiad normal
						}
					}, 3.0f, false);
				}
				break; 
			}
		}
		
	}
	ItemEquipado = EItemState::Empty;
}


void ARun4UrLifeCharacter::RecibirObjeto(EItemState NuevoEstado, int32 Cantidad)
{
	if (!HasAuthority()) return;

	
	if (ItemActual != NuevoEstado)
	{
		ItemActual = NuevoEstado;
		CantidadItems = 0; 
	}

	CantidadItems += Cantidad;
}

void ARun4UrLifeCharacter::Move(const FInputActionValue& Value)
{
	
	if (GetWorld())
	{
		AMiGameState* GS = Cast<AMiGameState>(GetWorld()->GetGameState());
		//Afewcta el movimiento a los jugadores si estan en la partida
		if (GS && !GS->bCarreraIniciada)
		{
			return; 
		}
	}
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ARun4UrLifeCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void ARun4UrLifeCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ARun4UrLifeCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ARun4UrLifeCharacter::DoJumpStart()
{
	// signal the character to jump
	Jump();
}

void ARun4UrLifeCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}

void ARun4UrLifeCharacter::DesactivarMovimiento()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		
		PC->SetIgnoreMoveInput(true);
		PC->SetIgnoreLookInput(true); 
	}
}

void ARun4UrLifeCharacter::ActivarMovimiento()
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC)
	{
		
		PC->SetIgnoreMoveInput(false);
		PC->SetIgnoreLookInput(false);
	}
}

void ARun4UrLifeCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled())
	{
		if (HUDClass)
		{
			UUserWidget* HUD = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
			if (HUD)
			{
				HUD->AddToViewport();
			}
		}
	}
	if (HasAuthority())
	{
		FTimerHandle TimerInicio;
		GetWorldTimerManager().SetTimer(TimerInicio, [this]()
		{
			if (AMiPlayerState* PS = Cast<AMiPlayerState>(GetPlayerState()))
			{
				PS->UltimoCheckpoint = GetActorLocation();
			}
		}, 0.2f, false);
	}
}

void ARun4UrLifeCharacter::MorirYReaparecer()
{
	Server_MorirYReaparecer();
}

void ARun4UrLifeCharacter::OnRep_CantidadItems()
{
}

void ARun4UrLifeCharacter::UsarItemEquipado_Implementation()
{
    if (CantidadItems <= 0 || ItemActual == EItemState::Empty) return;
	
    if (ItemActual == EItemState::Velocity)
    {
        if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
        {
            MoveComp->MaxWalkSpeed = 1000.f; 

            if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("¡Boost de velocidad activado por 5 segundos!"));

            
            FTimerHandle Timer_Velocidad;
            GetWorld()->GetTimerManager().SetTimer(Timer_Velocidad, [this, MoveComp]()
            {
                if (MoveComp)
                {
                    MoveComp->MaxWalkSpeed = 600.f; 
                    if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("Tu boost terminó."));
                }
            }, 5.0f, false); 
        }
    }
  
    else if (ItemActual == EItemState::Tramp)
    {
        
        for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
        {
            if (APlayerController* PC = It->Get())
            {
                if (PC != GetController()) // Si es el rival
                {
                    if (APawn* RivalPawn = PC->GetPawn())
                    {
                        if (ACharacter* RivalChar = Cast<ACharacter>(RivalPawn))
                        {
                            if (UCharacterMovementComponent* RivalMove = RivalChar->GetCharacterMovement())
                            {
                                RivalMove->MaxWalkSpeed = 200.f; 

                                if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("¡Rival ralentizado por 4 segundos!"));

                                
                                FTimerHandle Timer_Trampa;
                                GetWorld()->GetTimerManager().SetTimer(Timer_Trampa, [RivalMove]()
                                {
                                    if (RivalMove)
                                    {
                                        RivalMove->MaxWalkSpeed = 600.f; 
                                        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White, TEXT("El rival recuperó su velocidad."));
                                    }
                                }, 4.0f, false); 
                            }
                        }
                    }
                }
            }
        }
    }

   
    CantidadItems--;

    
    if (CantidadItems <= 0)
    {
        ItemActual = EItemState::Empty;
        CantidadItems = 0;
    }
}

void ARun4UrLifeCharacter::Client_EnfocarGanador_Implementation(AActor* ActorGanador)
{
	if (!ActorGanador) return;

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && IsLocallyControlled())
	{
		PC->SetIgnoreLookInput(true);
		
		FRotator RotacionFinal = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ActorGanador->GetActorLocation());
		
		float DistanciaAlPodio = FVector::Dist(GetActorLocation(), ActorGanador->GetActorLocation());
		
		if (DistanciaAlPodio < 300.f)
		{
			RotacionFinal.Yaw += 90.f; 
		}
		else
		{
			RotacionFinal.Pitch = 0.f;
		}
		
		PC->SetControlRotation(RotacionFinal);
	}
}

void ARun4UrLifeCharacter::Server_MorirYReaparecer_Implementation()
{
	if (AMiPlayerState* PS = Cast<AMiPlayerState>(GetPlayerState()))
	{
		if (GetCharacterMovement())
		{
			GetCharacterMovement()->StopMovementImmediately();
		}
		
		FVector UbicacionRespawn = PS->UltimoCheckpoint + FVector(0.f, 0.f, 100.f);
		FRotator RotacionRespawn = GetActorRotation(); 

		SetActorLocationAndRotation(UbicacionRespawn, RotacionRespawn, false, nullptr, ETeleportType::TeleportPhysics);

		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Orange, TEXT("Jugador Reaparecido"));
	}
}

void ARun4UrLifeCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ARun4UrLifeCharacter, ItemEquipado);
}
