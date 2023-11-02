// Copyright notice

#include "Weapons/Components/HitDetectionComponent.h"
// Engine
#include "Engine/World.h"
#include "Engine/HitResult.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
// Local
#include "Teams/TeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(HitDetectionComponent)

UHitDetectionComponent::UHitDetectionComponent()
{
	// As stated in Unreal Documentation, TG_PostPhysics is the perfect Tick Group for Line Traces and Hit Detection.
	PrimaryComponentTick.TickGroup = TG_PostPhysics;
	// We want this component to tick only during Hit Detection.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;
}


void UHitDetectionComponent::BeginPlay()
{
	Super::BeginPlay();

	// We will store this beforehand because Weapon's SkeletalMesh will never change.
	WeaponSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}


void UHitDetectionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// To avoid division by zero we will produce modulo operation only when TicksToSkip > 0.
	if (TicksToSkip > 0)
	{
		if (TickCounter % TicksToSkip == 0)
		{
			ProcessHitDetection();

			TickCounter = 0;
		}
		else
		{
			TickCounter++;
		}
	}
	else
	{
		ProcessHitDetection();
	}
}

void UHitDetectionComponent::StartHitDetection(AActor* InWeaponOwner, TArray<FName> CustomWeaponSockets, int32 HowManyTicksToSkip, bool ShowDebugTrace)
{
	WeaponOwner = InWeaponOwner;
	WeaponSockets = CustomWeaponSockets;
	TicksToSkip = HowManyTicksToSkip;
	bShowDebugTrace = ShowDebugTrace;

	TickCounter = 0;

	// For performance reasons, bones update is disabled on server side by default.
	// Therefore there will be no updates for LineTraces' location during animations.
	// To make it optimized, we will refresh bones only during Hit Detection.
	if (WeaponOwnerSkeletalMesh = WeaponOwner->FindComponentByClass<USkeletalMeshComponent>())
	{
		WeaponOwnerSkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	}

	SetComponentTickEnabled(true);
}

void UHitDetectionComponent::ProcessHitDetection()
{
	// This will be called everty tick during Hit Detection

	HitSameSocketAtDifferentFrames();
	HitDifferentSocketsAtSameFrame();

	UpdateLastSocketLocation();
}

void UHitDetectionComponent::EndHitDetection()
{
	SetComponentTickEnabled(false);

	if (WeaponOwnerSkeletalMesh)
	{
		WeaponOwnerSkeletalMesh->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;

		WeaponOwnerSkeletalMesh = nullptr;
	}

	ClearHitArray();
	ClearSocketLocationMap();
}

TArray<FName> UHitDetectionComponent::GetHitSockets() const
{
	TArray<FName> AllSockets = WeaponSkeletalMesh->GetAllSocketNames();
	TArray<FName> HitSockets;
	FString StringFilter = "HitSocket_";

	for (FName Socket : AllSockets)
	{
		if (Socket.ToString().Contains(StringFilter))
		{
			HitSockets.Add(Socket);
		}
	}

	return HitSockets;
}

void UHitDetectionComponent::HitSameSocketAtDifferentFrames()
{
	for (FName Socket : WeaponSockets)
	{
		// We won't process this function during first tick of Hit Detection.
		if (/*TraceStart*/ LastKnownSocketLocation.Find(Socket) == nullptr)
		{
			return;
		}

		TArray<FHitResult> OutHits;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(WeaponOwner);
		FVector TraceStart = *LastKnownSocketLocation.Find(Socket);
		FVector TraceEnd = WeaponSkeletalMesh->GetSocketLocation(Socket);


		GetWorld()->LineTraceMultiByObjectType(OutHits, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, TraceParams);
		if (bShowDebugTrace)
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f);
		}

		AddHitToHitArray(OutHits);
	}
}

void UHitDetectionComponent::HitDifferentSocketsAtSameFrame()
{
	for (auto Socket1 : WeaponSockets)
	{
		for (auto Socket2 : WeaponSockets)
		{
			// There is no meaning in LineTrace that starts and ends in the same socket.
			if (Socket1 == Socket2)
			{
				continue;
			}

			TArray<FHitResult> OutHits;
			FCollisionQueryParams TraceParams;
			TraceParams.AddIgnoredActor(WeaponOwner);
			FVector TraceStart = WeaponSkeletalMesh->GetSocketLocation(Socket1);
			FVector TraceEnd = WeaponSkeletalMesh->GetSocketLocation(Socket2);

			GetWorld()->LineTraceMultiByObjectType(OutHits, TraceStart, TraceEnd, ECollisionChannel::ECC_Pawn, TraceParams);
			if (bShowDebugTrace)
			{
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 2.0f);
			}

			AddHitToHitArray(OutHits);
		}
	}
}

void UHitDetectionComponent::UpdateLastSocketLocation()
{
	if (WeaponSockets.Num() > 0) {

		for (FName Socket : WeaponSockets)
		{
			LastKnownSocketLocation.Add(Socket, WeaponSkeletalMesh->GetSocketLocation(Socket));
		}
	}
}

void UHitDetectionComponent::AddHitToHitArray(TArray<FHitResult> HitArrayToAdd)
{
	// We only add new Actors to HitArray, store it during Hit Detection and clear it when it ends.
	for (const FHitResult& Hit : HitArrayToAdd)
	{
		if (!HitArray.ContainsByPredicate([&](const FHitResult& Inner) {  return Inner.GetActor() == Hit.GetActor(); }))
		{
			HitArray.Add(Hit);

			OnHitAdded(Hit);
		}
	}
}

void UHitDetectionComponent::OnHitAdded(FHitResult NewHit)
{
	AActor* HitActor = NewHit.GetActor();

	// We only want to broadcast Enemies' HitResult that are not behind wall or shield.
	if (IsInDifferentTeam(HitActor) && !IsBehindOccludingGeometry(HitActor))
	{
		OnActorHit.Broadcast(NewHit);
	}
}

bool UHitDetectionComponent::IsInDifferentTeam(const AActor* OtherActor)
{
	UTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<UTeamSubsystem>();

	ETeamComparison ComparisonResult = TeamSubsystem->CompareTeams(WeaponOwner, OtherActor);

	if (ComparisonResult == ETeamComparison::DifferentTeams)
	{
		return true;
	}

	return false;
}

bool UHitDetectionComponent::IsBehindOccludingGeometry(const AActor* OtherActor)
{
	FHitResult OutHit;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(WeaponOwner);
	TraceParams.AddIgnoredActor(OtherActor);
	FVector TraceStart = WeaponOwner->GetActorLocation() + FVector(0, 0, 20);
	FVector TraceEnd = OtherActor->GetActorLocation() + FVector(0, 0, 20);

	GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceParams);
	if (bShowDebugTrace)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 2.0f);
	}

	if (OutHit.GetActor() != nullptr)
	{
		return true;
	}

	return false;
}

void UHitDetectionComponent::ClearHitArray()
{
	HitArray.Empty();
}

void UHitDetectionComponent::ClearSocketLocationMap()
{
	LastKnownSocketLocation.Empty();
}