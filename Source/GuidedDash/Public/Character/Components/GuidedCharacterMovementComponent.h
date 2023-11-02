// Copyright notice

#pragma once

#include "GameFramework/CharacterMovementComponent.h"
#include "NativeGameplayTags.h"

#include "GuidedCharacterMovementComponent.generated.h"

GUIDEDDASH_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Gameplay_MovementStopped);

/*
 *	FCharacterGroundInfo
 *
 *	Information about the ground under the character. It only gets updated as needed.
 */
USTRUCT(BlueprintType)
struct FCharacterGroundInfo
{
	GENERATED_BODY()

	FCharacterGroundInfo()
		: LastUpdateFrame(0)
		, GroundDistance(0.0f)
	{}

	uint64 LastUpdateFrame;

	UPROPERTY(BlueprintReadOnly)
	FHitResult GroundHitResult;

	UPROPERTY(BlueprintReadOnly)
	float GroundDistance;
};

/*
 *	UGuidedCharacterMovementComponent
 *
 *	The base character movement component class used by this project.
 */
UCLASS(Config = Game)
class GUIDEDDASH_API UGuidedCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	class FGuidedSavedMove : public FSavedMove_Character
	{
	public:

		typedef FSavedMove_Character Super;

		// Resets all saved variables.
		virtual void Clear() override;

		// Store input commands in the compressed flags.
		virtual uint8 GetCompressedFlags() const override;

		// This is used to check whether or not two moves can be combined into one.
		// Basically you just check to make sure that the saved variables are the same.
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* Character, float MaxDelta) const override;

		// Sets up the move before sending it to the server. 
		virtual void SetMoveFor(ACharacter* Character, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character& ClientData) override;

		// Sprint
		uint8 SavedRequestToStartSprinting : 1;
	};

	class FGuidedNetworkPredictionData_Client : public FNetworkPredictionData_Client_Character
	{
	public:
		FGuidedNetworkPredictionData_Client(const UCharacterMovementComponent& ClientMovement) 
			: FNetworkPredictionData_Client_Character(ClientMovement)
		{
		}

		// Allocates a new copy of our custom saved move
		virtual FSavedMovePtr AllocateNewMove() override;
	};
	
public:
	// Returns the current ground info.  Calling this will update the ground info if it's out of date.
	UFUNCTION(BlueprintCallable, Category = "Guided|CharacterMovement")
	const FCharacterGroundInfo& GetGroundInfo();

	virtual void SimulateMovement(float DeltaTime) override;

	virtual bool CanAttemptJump() const override;

	void SetReplicatedAcceleration(const FVector& InAcceleration);

	virtual FRotator GetDeltaRotation(float DeltaTime) const override;
	virtual float GetMaxSpeed() const override;

	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual class FNetworkPredictionData_Client* GetPredictionData_Client() const override;

	UFUNCTION(BlueprintCallable, Category = "Guided|Sprint")
	void StartSprinting();
	UFUNCTION(BlueprintCallable, Category = "Guided|Sprint")
	void StopSprinting();

protected:
	// Cached ground info for the character.  Do not access this directly!  It's only updated when accessed via GetGroundInfo().
	FCharacterGroundInfo CachedGroundInfo;

	UPROPERTY(Transient)
	bool bHasReplicatedAcceleration = false;

	uint8 RequestToStartSprinting : 1;
};
