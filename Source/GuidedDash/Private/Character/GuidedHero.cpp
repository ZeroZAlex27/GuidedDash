// Copyright notice

#include "Character/GuidedHero.h"
// Engine
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Local
#include "GuidedGameplayTags.h"
#include "Input/GuidedInputConfig.h"
#include "Input/GuidedInputComponent.h"
#include "Character/PawnData.h"
#include "AbilitySystem/GuidedAbilitySystemComponent.h"
#include "Character/Components/InventoryComponent.h"
#include "Character/Components/EquipmentComponent.h"
#include "Inventory/InventoryItemDefinition.h"
#include "Inventory/InventoryItemInstance.h"
#include "Inventory/InventoryFragment_EquippableItem.h"

AGuidedHero::AGuidedHero()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArmComponent->TargetArmLength = 300;
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComponent"));
}

void AGuidedHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = PC->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();

	if (ensureAlwaysMsgf(IMC, TEXT("Input Mapping Context in Hero Character is missing!")))
	{
		Subsystem->AddMappingContext(IMC, 0);
	}

	if (const UGuidedInputConfig* InputConfig = PawnData->InputConfig)
	{
		UGuidedInputComponent* IC = Cast<UGuidedInputComponent>(PlayerInputComponent);
		if (ensureMsgf(IC, TEXT("Unexpected Input Component class! The Gameplay Abilities will not be bound to their inputs. Change the input component to UGuidedInputComponent or a subclass of it.")))
		{
			// This is where we actually bind and input action to a gameplay tag, which means that Gameplay Ability Blueprints will
			// be triggered directly by these input actions Triggered events. 
			TArray<uint32> BindHandles;
			IC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, /*out*/ BindHandles);

			IC->BindNativeAction(InputConfig, GuidedGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, /*bLogIfNotFound=*/ false);
			IC->BindNativeAction(InputConfig, GuidedGameplayTags::InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, /*bLogIfNotFound=*/ false);
			IC->BindNativeAction(InputConfig, GuidedGameplayTags::InputTag_Look_Stick, ETriggerEvent::Triggered, this, &ThisClass::Input_LookStick, /*bLogIfNotFound=*/ false);
			IC->BindNativeAction(InputConfig, GuidedGameplayTags::InputTag_Crouch, ETriggerEvent::Started, this, &ThisClass::Input_Crouch, /*bLogIfNotFound=*/ false);
		}
	}

	if (UGuidedAbilitySystemComponent* ASC = GetGuidedAbilitySystemComponent())
	{
		ASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	}
}

void AGuidedHero::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (UGuidedAbilitySystemComponent* ASC = GetGuidedAbilitySystemComponent())
	{
		ASC->AbilityInputTagPressed(InputTag);
	}
}

void AGuidedHero::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UGuidedAbilitySystemComponent* ASC = GetGuidedAbilitySystemComponent())
	{
		ASC->AbilityInputTagReleased(InputTag);
	}
}

void AGuidedHero::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (Value.X != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Value.X);
	}

	if (Value.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Value.Y);
	}

	MakeNoise(1.0f, this, GetActorLocation());
}

void AGuidedHero::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y);
	}
}

void AGuidedHero::Input_LookStick(const FInputActionValue& InputActionValue)
{
	const FVector2D Value = InputActionValue.Get<FVector2D>();

	const UWorld* World = GetWorld();
	check(World);

	if (Value.X != 0.0f)
	{
		AddControllerYawInput(Value.X * 300.0f * World->GetDeltaSeconds());
	}

	if (Value.Y != 0.0f)
	{
		AddControllerPitchInput(Value.Y * 165.0f * World->GetDeltaSeconds());
	}
}

void AGuidedHero::Input_Crouch(const FInputActionValue& InputActionValue)
{
	ToggleCrouch();
}

void AGuidedHero::AddInitialEquipment(TArray<TSubclassOf<UInventoryItemDefinition>> ItemsToEquip)
{
	// TODO: Move to Equipment component

	for (TSubclassOf<UInventoryItemDefinition> ItemDef : ItemsToEquip)
	{
		if (ItemDef != nullptr)
		{
			UInventoryItemInstance* Instance = InventoryComponent->AddItemDefinition(ItemDef, 1);

			if (const UInventoryFragment_EquippableItem* EquipInfo = Instance->FindFragmentByClass<UInventoryFragment_EquippableItem>())
			{
				if (TSubclassOf<UEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition)
				{
					if (EquipDef != nullptr)
					{
						EquipmentComponent->EquipItem(EquipDef);
					}
				}
			}
		}
	}
}
