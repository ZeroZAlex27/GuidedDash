// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/GuidedGameMode.h"
// Local
#include "GameModes/GuidedGameState.h"
#include "GameModes/GameStateComponents/PlayerSpawningComponent.h"
#include "GameModes/GameStateComponents/BotSpawningComponent.h"
#include "Player/GuidedPlayerController.h"
#include "Player/GuidedPlayerState.h"
#include "Player/BotController.h"
#include "Character/GuidedCharacter.h"
#include "UI/GuidedHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedGameMode)

AGuidedGameMode::AGuidedGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = AGuidedGameState::StaticClass();
	PlayerControllerClass = AGuidedPlayerController::StaticClass();
	PlayerStateClass = AGuidedPlayerState::StaticClass();
	DefaultPawnClass = AGuidedCharacter::StaticClass();
	HUDClass = AGuidedHUD::StaticClass();
}

APawn* AGuidedGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;	// Never save the default player pawns into a map.
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			SpawnedPawn->FinishSpawning(SpawnTransform);

			return SpawnedPawn;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn of class [%s] at [%s]."), *GetNameSafe(PawnClass), *SpawnTransform.ToHumanReadableString());
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Game mode was unable to spawn Pawn due to NULL pawn class."));
	}

	return nullptr;
}

bool AGuidedGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	// We never want to use the start spot, always use the spawn management component.
	return false;
}

AActor* AGuidedGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (UPlayerSpawningComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UPlayerSpawningComponent>())
	{
		return PlayerSpawningComponent->ChoosePlayerStart(Player);
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AGuidedGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	if (UPlayerSpawningComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UPlayerSpawningComponent>())
	{
		PlayerSpawningComponent->FinishRestartPlayer(NewPlayer, StartRotation);
	}

	Super::FinishRestartPlayer(NewPlayer, StartRotation);
}

bool AGuidedGameMode::PlayerCanRestart_Implementation(APlayerController* Player)
{
	return ControllerCanRestart(Player);
}

bool AGuidedGameMode::ControllerCanRestart(AController* Controller)
{
	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		if (!Super::PlayerCanRestart_Implementation(PC))
		{
			return false;
		}
	}
	else
	{
		// Bot version of Super::PlayerCanRestart_Implementation
		if ((Controller == nullptr) || Controller->IsPendingKillPending())
		{
			return false;
		}
	}

	if (UPlayerSpawningComponent* PlayerSpawningComponent = GameState->FindComponentByClass<UPlayerSpawningComponent>())
	{
		return PlayerSpawningComponent->ControllerCanRestart(Controller);
	}

	return true;
}

void AGuidedGameMode::InitGameState()
{
	Super::InitGameState();

	if (AGuidedGameState* GS = GetGameState<AGuidedGameState>())
	{
		GS->CreateTeamsAndBots();
	}
}

void AGuidedGameMode::GenericPlayerInitialization(AController* NewPlayer)
{
	Super::GenericPlayerInitialization(NewPlayer);

	if (UBotSpawningComponent* BotSpawningComponent = GameState->FindComponentByClass<UBotSpawningComponent>())
	{
		if (NewPlayer->IsPlayerController())
		{
			BotSpawningComponent->RemoveOneBot();
		}
	}

	OnGameModePlayerInitialized.Broadcast(this, NewPlayer);
}

void AGuidedGameMode::RequestPlayerRestartNextFrame(AController* Controller, bool bForceReset)
{
	if (bForceReset && (Controller != nullptr))
	{
		Controller->Reset();
	}

	if (APlayerController* PC = Cast<APlayerController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(PC, &APlayerController::ServerRestartPlayer_Implementation);
	}
	else if (ABotController* BotController = Cast<ABotController>(Controller))
	{
		GetWorldTimerManager().SetTimerForNextTick(BotController, &ABotController::ServerRestartController);
	}
}

bool AGuidedGameMode::UpdatePlayerStartSpot(AController* Player, const FString& Portal, FString& OutErrorMessage)
{
	// Do nothing, we'll wait until PostLogin when we try to spawn the player for real.
	// Doing anything right now is no good, systems like team assignment haven't even occurred yet.
	return true;
}

void AGuidedGameMode::FailedToRestartPlayer(AController* NewPlayer)
{
	Super::FailedToRestartPlayer(NewPlayer);

	// If we tried to spawn a pawn and it failed, lets try again *note* check if there's actually a pawn class
	// before we try this forever.
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APlayerController* NewPC = Cast<APlayerController>(NewPlayer))
		{
			// If it's a player don't loop forever, maybe something changed and they can no longer restart if so stop trying.
			if (PlayerCanRestart(NewPC))
			{
				RequestPlayerRestartNextFrame(NewPlayer, false);
			}
			else
			{
				UE_LOG(LogTemp, Verbose, TEXT("FailedToRestartPlayer(%s) and PlayerCanRestart returned false, so we're not going to try again."), *GetPathNameSafe(NewPlayer));
			}
		}
		else
		{
			RequestPlayerRestartNextFrame(NewPlayer, false);
		}
	}
	else
	{
		UE_LOG(LogTemp, Verbose, TEXT("FailedToRestartPlayer(%s) but there's no pawn class so giving up."), *GetPathNameSafe(NewPlayer));
	}
}
