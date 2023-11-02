// Copyright notice

#pragma once

#include "Components/ActorComponent.h"

#include "PlayerSpawningComponent.generated.h"

class AController;
class APlayerStart;
class AGuidedPlayerStart;
class AActor;

/*
*	UPlayerSpawningComponent
* 
*	GameState component used for spawning players
*/
UCLASS(Blueprintable)
class GUIDEDDASH_API UPlayerSpawningComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerSpawningComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

protected:
	// Utility
	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<AGuidedPlayerStart*>& FoundStartPoints) const;

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<AGuidedPlayerStart*>& PlayerStarts);
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = OnFinishRestartPlayer))
	void K2_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

private:
	AActor* ChoosePlayerStart(AController* Player);
	bool ControllerCanRestart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);
	friend class AGuidedGameMode;

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AGuidedPlayerStart>> CachedPlayerStarts;

private:
	void OnLevelAdded(ULevel* InLevel, UWorld* InWorld);
	void HandleOnActorSpawned(AActor* SpawnedActor);

#if WITH_EDITOR
	APlayerStart* FindPlayFromHereStart(AController* Player);
#endif	
};
