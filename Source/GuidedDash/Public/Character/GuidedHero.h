// Copyright notice

#pragma once

#include "Character/GuidedCharacter.h"

#include "GameplayAbilitySpecHandle.h"
#include "GuidedHero.generated.h"

class UInputComponent;
class UInputMappingContext;
class UCameraComponent;
class USpringArmComponent;
class UInventoryComponent;
class UEquipmentComponent;
class UInventoryItemDefinition;
struct FGameplayTag;
struct FInputActionValue;

/*
 *	AGuidedHero
 * 
 *	Base character allocated for players
 */
UCLASS()
class GUIDEDDASH_API AGuidedHero : public AGuidedCharacter
{
	GENERATED_BODY()

public:
	AGuidedHero();
	
protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintCallable, Category = "Guided|Eqipment")
	void AddInitialEquipment(TArray<TSubclassOf<UInventoryItemDefinition>> ItemsToEquip);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guided|Inventory")
	TObjectPtr<UInventoryComponent> InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Guided|Equipment")
	TObjectPtr<UEquipmentComponent> EquipmentComponent;
};
