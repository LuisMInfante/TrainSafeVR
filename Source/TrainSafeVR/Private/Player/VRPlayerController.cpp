// Copyright © 2024 Luis M. Infante
// Licensed under the GNU General Public License v3.0 (GPLv3).
// See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#include "Player/VRPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

AVRPlayerController::AVRPlayerController()
{
}

void AVRPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Check if HMD is enabled
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		// Set the tracking origin to Stage (floor level)
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Stage);

		// Execute PixelDensity Console Command
		// Referenced here: "https://www.unrealengine.com/en-US/blog/significant-changes-coming-to-vr-resolution-settings-in-4-19"
		const FString PixelDensityCommand = TEXT("vr.PixelDensity 1.0");
		ConsoleCommand(PixelDensityCommand);
	}
}

void AVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	/* Movement */
	EnhancedInputComponent->BindAction(MoveHorizontalAction, ETriggerEvent::Triggered, this, &AVRPlayerController::MoveHorizontal);
	EnhancedInputComponent->BindAction(MoveVerticalAction, ETriggerEvent::Triggered, this, &AVRPlayerController::MoveVertical);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AVRPlayerController::Jump);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AVRPlayerController::Sprint);

	/* Viewing */
	EnhancedInputComponent->BindAction(SnapTurnAction, ETriggerEvent::Triggered, this, &AVRPlayerController::SnapTurn);

	/* Grabbing */
	EnhancedInputComponent->BindAction(GrabLeftAction, ETriggerEvent::Triggered, this, &AVRPlayerController::GrabLeft);
	EnhancedInputComponent->BindAction(ReleaseLeftAction, ETriggerEvent::Triggered, this, &AVRPlayerController::ReleaseLeft);
	EnhancedInputComponent->BindAction(GrabRightAction, ETriggerEvent::Triggered, this, &AVRPlayerController::GrabRight);
	EnhancedInputComponent->BindAction(ReleaseRightAction, ETriggerEvent::Triggered, this, &AVRPlayerController::ReleaseRight);

	/* Menu */
	EnhancedInputComponent->BindAction(LeftMenuAction, ETriggerEvent::Triggered, this, &AVRPlayerController::ToggleLeftMenu);
	EnhancedInputComponent->BindAction(RightMenuAction, ETriggerEvent::Triggered, this, &AVRPlayerController::ToggleRightMenu);
}

void AVRPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	// Get Local Player and Enhanced Input Subsystem
	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		if (Subsystem)
		{
			// Clear existing mapping contexts
			Subsystem->ClearAllMappings();

			// Add Default Mapping Context
			if (VRMappingContext)
			{
				Subsystem->AddMappingContext(VRMappingContext, 0);
			}

			// Add Hands Mapping Context
			if (HandsMappingContext)
			{
				Subsystem->AddMappingContext(HandsMappingContext, 0);
			}
		}
	}
}

void AVRPlayerController::MoveHorizontal(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::MoveVertical(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::Jump(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::Sprint(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::SnapTurn(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::GrabLeft(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::ReleaseLeft(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::GrabRight(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::ReleaseRight(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::ToggleLeftMenu(const FInputActionValue& InputActionValue)
{
}

void AVRPlayerController::ToggleRightMenu(const FInputActionValue& InputActionValue)
{
}
