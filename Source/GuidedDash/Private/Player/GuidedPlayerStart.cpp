// Copyright notice

#include "Player/GuidedPlayerStart.h"
// Engine
#include "Engine/World.h"
#include "GameFramework/GameModeBase.h"
#include "TimerManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GuidedPlayerStart)

EPlayerStartLocationOccupancy AGuidedPlayerStart::GetLocationOccupancy(AController* const ControllerPawnToFit) const
{
	UWorld* const World = GetWorld();
	if (HasAuthority() && World)
	{
		if (AGameModeBase* AuthGameMode = World->GetAuthGameMode())
		{
			TSubclassOf<APawn> PawnClass = AuthGameMode->GetDefaultPawnClassForController(ControllerPawnToFit);
			const APawn* const PawnToFit = PawnClass ? GetDefault<APawn>(PawnClass) : nullptr;

			FVector ActorLocation = GetActorLocation();
			const FRotator ActorRotation = GetActorRotation();

			if (!World->EncroachingBlockingGeometry(PawnToFit, ActorLocation, ActorRotation, nullptr))
			{
				return EPlayerStartLocationOccupancy::Empty;
			}
			else if (World->FindTeleportSpot(PawnToFit, ActorLocation, ActorRotation))
			{
				return EPlayerStartLocationOccupancy::Partial;
			}
		}
	}

	return EPlayerStartLocationOccupancy::Full;
}

bool AGuidedPlayerStart::IsClaimed() const
{
	return ClaimingController != nullptr;
}

bool AGuidedPlayerStart::TryClaim(AController* OccupyingController)
{
	if (OccupyingController != nullptr && !IsClaimed())
	{
		ClaimingController = OccupyingController;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(ExpirationTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::CheckUnclaimed), ExpirationCheckInterval, true);
		}
		return true;
	}
	return false;
}

void AGuidedPlayerStart::CheckUnclaimed()
{
	if (ClaimingController != nullptr && ClaimingController->GetPawn() != nullptr && GetLocationOccupancy(ClaimingController) == EPlayerStartLocationOccupancy::Empty)
	{
		ClaimingController = nullptr;
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().ClearTimer(ExpirationTimerHandle);
		}
	}
}
