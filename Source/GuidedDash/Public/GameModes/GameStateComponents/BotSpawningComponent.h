// Copyright notice

#pragma once

#include "Components/ActorComponent.h"

#include "BotSpawningComponent.generated.h"

class UPawnData;
class AAIController;

/*
*	UBotSpawningComponent
*
*	GameState component used for spawning bots
*/
UCLASS(Blueprintable)
class GUIDEDDASH_API UBotSpawningComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = Teams)
	int32 NumBotsToCreate = 10;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TArray<FString> RandomBotNames;

	TArray<FString> RemainingBotNames;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnedBotList;

#if WITH_SERVER_CODE
public:
	virtual void ServerCreateBots();

	virtual void SpawnOneBot();
	virtual void RemoveOneBot();

	FString CreateBotName(int32 PlayerIndex);
#endif
};
