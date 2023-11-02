// Copyright notice

#pragma once

#include "Components/ActorComponent.h"

#include "HitDetectionComponent.generated.h"

struct FHitResult;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActorHit, FHitResult, NewHit);

/*
*	UHitDetectionComponent
* 
*	Weapon component used for hit detection by this project.
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GUIDEDDASH_API UHitDetectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHitDetectionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// "StartHitDetection" and "EndHitDetection" needs to be called manually from Attack ability.
	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
	void StartHitDetection(AActor* InWeaponOwner, TArray<FName> CustomWeaponSockets, int32 HowManyTicksToSkip = 5, bool ShowDebugTrace = false);

	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
	void EndHitDetection();

	// This is an Event Dispatcher that needs to be assigned in the Attack ability.
	UPROPERTY(BlueprintAssignable, Category = "Hit Detection")
	FOnActorHit OnActorHit;

	// This is useful function for gathering sockets with custom naming "HitSocket_".
	// You can provide your custom sockets array in "StartHitDetection" if you don't want to use it.
	UFUNCTION(BlueprintCallable, Category = "Hit Detection")
	TArray<FName> GetHitSockets() const;

protected:
	virtual void BeginPlay() override;

private:
	void ProcessHitDetection();

	void HitSameSocketAtDifferentFrames();

	void HitDifferentSocketsAtSameFrame();

	void UpdateLastSocketLocation();

	void AddHitToHitArray(TArray<FHitResult> HitArrayToAdd);

	void OnHitAdded(FHitResult NewHit);

	bool IsInDifferentTeam(const AActor* OtherActor);
	bool IsBehindOccludingGeometry(const AActor* OtherActor);

	void ClearHitArray();
	void ClearSocketLocationMap();

private:
	UPROPERTY()
	TObjectPtr<AActor> WeaponOwner;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> WeaponOwnerSkeletalMesh;

	UPROPERTY()
	TObjectPtr<USkeletalMeshComponent> WeaponSkeletalMesh;

	TArray<FName> WeaponSockets;

	TMap<FName, FVector> LastKnownSocketLocation;

	TArray<FHitResult> HitArray;

	int32 TicksToSkip;
	int32 TickCounter;

	bool bShowDebugTrace;
};
