// Copyright © 2024 Luis M. Infante
// Licensed under the GNU General Public License v3.0 (GPLv3).
// See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VRPlayerController.generated.h"

/*
	* Handle Player Input
*/

class UNiagaraSystem;
struct FPredictProjectilePathPointData;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ACharacter;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputComponent;
struct FTimerHandle;

UCLASS()
class TRAINSAFEVR_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AVRPlayerController();
	virtual void Tick(float DeltaTime) override;
	
	FORCEINLINE TObjectPtr<UInputMappingContext> GetDefaultMappingContext() { return VRMappingContext; }
	FORCEINLINE TObjectPtr<UInputMappingContext> GetHandsMappingContext() { return HandsMappingContext; }
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	/* Utility */
	void UpdateCapsulePositionAndRotation();
	void UpdateCapsuleHeight();
	
	/* Movement */
	void Move(const FInputActionValue& InputActionValue);
	void Sprint(const FInputActionValue& InputActionValue);

	/* Sprinting */
	void OnSprintStarted();
	void OnSprintCompleted();
	void DepleteStamina(float DeltaTime);
	void RegenerateStamina();

	void StartStaminaRegenTimer();
	void StopStaminaRegenTimer();

	/* Teleporting */
	
	/* Viewing */
	void SnapTurn(const FInputActionValue& InputActionValue);

	/* Grabbing */
	void GrabLeft(const FInputActionValue& InputActionValue);
	void ReleaseLeft(const FInputActionValue& InputActionValue);
	void GrabRight(const FInputActionValue& InputActionValue);
	void ReleaseRight(const FInputActionValue& InputActionValue);

	/* Menu */
	void ToggleLeftMenu(const FInputActionValue& InputActionValue);
	void ToggleRightMenu(const FInputActionValue& InputActionValue);
	
private:
	/* Player Properties */
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Walk Speed"))
	float WalkSpeed = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Movement", meta = (DisplayName = "Sprint Speed"))
	float SprintSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category = "Camera", meta = (DisplayName = "Player Height"))
	float DefaultPlayerHeight = 180.0f;
	UPROPERTY(EditAnywhere, Category = "Turning", meta = (DisplayName = "Snap Turn Angle"))
	float SnapTurnDegrees = 45.0f;
	
	/* Stamina Properties */
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (DisplayName = "Max Stamina"))
	float MaxStamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (DisplayName = "Current Stamina"))
	float Stamina = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (DisplayName = "Stamina Depletion Rate"))
	float StaminaDepletionRate = 35.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (DisplayName = "Stamina Regeneration Rate"))
	float StaminaRegenRate = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Stamina", meta = (DisplayName = "Cooldown Time"))
	bool bIsStaminaOnCooldown = false;

	/* Teleport Properties */
	
	/* Timer Handle */
	FTimerHandle StaminaRegenTimerHandle;

	/* Enhanced Input System */
	TObjectPtr<ACharacter> ControlledCharacter;
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem;
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

	/* Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Default Mapping Context"))
	TObjectPtr<UInputMappingContext> VRMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Hands Mapping Context"))
	TObjectPtr<UInputMappingContext> HandsMappingContext;
	
	/* Movement */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Move Action"))
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Jump Action"))
	TObjectPtr<UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Sprint Action"))
	TObjectPtr<UInputAction> SprintAction;

	/* Viewing */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Snap Turn Action"))
	TObjectPtr<UInputAction> SnapTurnAction;

	/* Grabbing */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Grab Left Action"))
	TObjectPtr<UInputAction> GrabLeftAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Release Left Action"))
	TObjectPtr<UInputAction> ReleaseLeftAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Grab Right Action"))
	TObjectPtr<UInputAction> GrabRightAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Release Right Action"))
	TObjectPtr<UInputAction> ReleaseRightAction;

	/* Menu */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Left Menu Action"))
	TObjectPtr<UInputAction> LeftMenuAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Right Menu Action"))
	TObjectPtr<UInputAction> RightMenuAction;
};
