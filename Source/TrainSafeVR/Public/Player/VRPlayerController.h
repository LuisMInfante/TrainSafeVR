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

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class ACharacter;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputComponent;

UCLASS()
class TRAINSAFEVR_API AVRPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AVRPlayerController();
	
	FORCEINLINE TObjectPtr<UInputMappingContext> GetDefaultMappingContext() { return VRMappingContext; }
	FORCEINLINE TObjectPtr<UInputMappingContext> GetHandsMappingContext() { return HandsMappingContext; }
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* InPawn) override;

private:
	/* Movement */
	void MoveHorizontal(const FInputActionValue& InputActionValue);
	void MoveVertical(const FInputActionValue& InputActionValue);
	void Jump(const FInputActionValue& InputActionValue);
	void Sprint(const FInputActionValue& InputActionValue);
	
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
	TObjectPtr<ACharacter> ControlledCharacter;
	TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem;
	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent;

	/* Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Default Mapping Context"))
	TObjectPtr<UInputMappingContext> VRMappingContext;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Hands Mapping Context"))
	TObjectPtr<UInputMappingContext> HandsMappingContext;
	

	/* Movement */
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Horizontal Move Action"))
	TObjectPtr<UInputAction> MoveHorizontalAction;
	UPROPERTY(EditAnywhere, Category = "Input", meta = (DisplayName = "Vertical Move Action"))
	TObjectPtr<UInputAction> MoveVerticalAction;
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
