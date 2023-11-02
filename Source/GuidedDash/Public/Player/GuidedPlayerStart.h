// Copyright notice

#pragma once

#include "GameFramework/PlayerStart.h"
#include "GameplayTagContainer.h"

#include "GuidedPlayerStart.generated.h"

class AController;

enum class EPlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

/*
 *	AGuidedPlayerStart
 *
 *	The base player start that can be used by a lot of modes.
 */
UCLASS(Config = Game)
class GUIDEDDASH_API AGuidedPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	const FGameplayTagContainer& GetGameplayTags() { return StartPointTags; }

	EPlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	// Did this player start get claimed by a controller already?
	bool IsClaimed() const;

	// If this PlayerStart was not claimed, claim it for ClaimingController
	bool TryClaim(AController* OccupyingController);

protected:
	// Check if this PlayerStart is still claimed
	void CheckUnclaimed();

	// The controller that claimed this PlayerStart
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	// Interval in which we'll check if this player start is not colliding with anyone anymore
	UPROPERTY(EditDefaultsOnly, Category = "Player Start Claiming")
	float ExpirationCheckInterval = 1.f;

	// Tags to identify this player start
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer StartPointTags;

	// Handle to track expiration recurring timer
	FTimerHandle ExpirationTimerHandle;
};
