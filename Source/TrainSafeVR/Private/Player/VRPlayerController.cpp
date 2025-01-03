// Copyright © 2024 Luis M. Infante
// Licensed under the GNU General Public License v3.0 (GPLv3).
// See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#include "Player/VRPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "NiagaraDataInterfaceArrayFunctionLibrary.h"
#include "NiagaraSystem.h"

AVRPlayerController::AVRPlayerController()
{
	bReplicates = true;
}

void AVRPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Height could realistically change every frame depending on head movement
	UpdateCapsuleHeight();
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

	/* Initialize Player */
	UpdateCapsulePositionAndRotation();

	// Set Maximum Walk Speed
	if (ControlledCharacter)
	{
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AVRPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	/* Movement */
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVRPlayerController::Move);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AVRPlayerController::OnSprintStarted);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AVRPlayerController::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AVRPlayerController::OnSprintCompleted);

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

	// Get Owner Pawn
	ControlledCharacter = GetPawn<ACharacter>();
	
	// Initialize Input for Server
	Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
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

	UpdateCapsulePositionAndRotation();
}

void AVRPlayerController::UpdateCapsulePositionAndRotation()
{
	if (!ControlledCharacter) return;

	// Get references to Camera and Capsule components
	const TObjectPtr<UCameraComponent> Camera = ControlledCharacter->FindComponentByClass<UCameraComponent>();
	const TObjectPtr<UCapsuleComponent> Capsule = ControlledCharacter->FindComponentByClass<UCapsuleComponent>();

	if (!Camera || !Capsule) return;

	// Calculate the camera's offset relative to the capsule
	const FVector CameraLocation = Camera->GetComponentLocation();
	const FVector CapsuleLocation = Capsule->GetComponentLocation();
	FVector Offset = CameraLocation - CapsuleLocation;

	// Update the actor's world offset
	Offset.X *= -1.0f; // Negate X axis
	Offset.Y *= -1.0f; // Negate Y axis
	ControlledCharacter->AddActorWorldOffset(FVector(Offset.X, Offset.Y, 0.0f), false);

	// Update VR Origin offset
	if (USceneComponent* VROrigin = ControlledCharacter->FindComponentByClass<USceneComponent>())
	{
		VROrigin->AddWorldOffset(FVector(Offset.X, Offset.Y, 0.0f), false);
	}
}

void AVRPlayerController::UpdateCapsuleHeight()
{
	if (!ControlledCharacter) return;
	
	// Get references to Camera and Capsule components
	const TObjectPtr<UCameraComponent> Camera = ControlledCharacter->FindComponentByClass<UCameraComponent>();
	const TObjectPtr<UCapsuleComponent> Capsule = ControlledCharacter->FindComponentByClass<UCapsuleComponent>();

	if (!Camera || !Capsule) return;

	// Calculate Z-axis offset between Camera and Capsule
	const float CameraZ = Camera->GetComponentLocation().Z;
	const float CapsuleZ = Capsule->GetComponentLocation().Z;
	const float ZOffset = CameraZ - CapsuleZ;
	
	// Update Capsule Half Height
	const float MinimumHalfHeight = DefaultPlayerHeight / 2.0f;
	const float NewHalfHeight = FMath::Max((ZOffset / 2.0f), MinimumHalfHeight);
	Capsule->SetCapsuleHalfHeight(NewHalfHeight, true);

	// Align VR Origin to the new Capsule Half Height
	if (const TObjectPtr<USceneComponent> VROrigin = ControlledCharacter->FindComponentByClass<USceneComponent>())
	{
		// Set the VR Origin relative to the top of the capsule
		const FVector NewOriginLocation = FVector(0.0f, 0.0f, NewHalfHeight);
		VROrigin->SetRelativeLocation(NewOriginLocation);
	}
}

void AVRPlayerController::Move(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
	
	// Retrieve the input vector (X for horizontal, Y for vertical)
	const FVector2D InputAxisVec2 = InputActionValue.Get<FVector2D>();

	// Get the Camera Component
	const TObjectPtr<UCameraComponent> Camera = ControlledCharacter->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	// Get the Camera's forward and right vectors
	const FVector ForwardVector = Camera->GetForwardVector();
	const FVector RightVector = Camera->GetRightVector();

	// Apply movement based on the Camera's orientation
	ControlledCharacter->AddMovementInput(ForwardVector, InputAxisVec2.Y);
	ControlledCharacter->AddMovementInput(RightVector, InputAxisVec2.X);
}

void AVRPlayerController::Sprint(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter || ControlledCharacter->GetCharacterMovement()->IsFalling() || Stamina <= 0.0f)
	{
		bIsStaminaOnCooldown = true;
		ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
		return;
	}

	const bool bIsSprinting = InputActionValue.Get<bool>();
	if (bIsSprinting && Stamina > 0.0f)
	{
		DepleteStamina(GetWorld()->GetDeltaSeconds());
	}
}

void AVRPlayerController::OnSprintStarted()
{
	if (!ControlledCharacter) return;

	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	StopStaminaRegenTimer();
}

void AVRPlayerController::OnSprintCompleted()
{
	if (!ControlledCharacter) return;

	ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	StartStaminaRegenTimer();
	bIsStaminaOnCooldown = true;
}

void AVRPlayerController::DepleteStamina(float DeltaTime)
{
	if (!ControlledCharacter) return;
	
	Stamina -= StaminaDepletionRate * DeltaTime;
	Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);

	if (Stamina <= 0.0f)
	{
		bIsStaminaOnCooldown = true;
		StartStaminaRegenTimer();
	}
}

void AVRPlayerController::RegenerateStamina()
{
	if (bIsStaminaOnCooldown)
	{
		Stamina += StaminaRegenRate * GetWorld()->GetDeltaSeconds();
		Stamina = FMath::Clamp(Stamina, 0.0f, MaxStamina);

		if (Stamina >= MaxStamina)
		{
			bIsStaminaOnCooldown = false;
		}
	}
}

void AVRPlayerController::StartStaminaRegenTimer()
{
	GetWorldTimerManager().SetTimer(StaminaRegenTimerHandle, this, &AVRPlayerController::RegenerateStamina, 0.1f, true);
}

void AVRPlayerController::StopStaminaRegenTimer()
{
	GetWorldTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void AVRPlayerController::SnapTurn(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;

	// Retrieve the snap turn direction (-1 for Left, 1 for Right)
	const float SnapDirection = InputActionValue.Get<float>();
	if (FMath::IsNearlyZero(SnapDirection)) return;

	// Turn based on Snapping Degree and Direction
	const float TurnAngle = SnapDirection > 0.0f ? SnapTurnDegrees : -SnapTurnDegrees;

	// Rotate Player
	const FRotator CurrentRotation = ControlledCharacter->GetActorRotation();
	const FRotator NewRotation = FRotator(0.0f, CurrentRotation.Yaw + TurnAngle, 0.0f);
	ControlledCharacter->SetActorRotation(NewRotation);

	/* Adjust Camera */
	const TObjectPtr<UCameraComponent> Camera = ControlledCharacter->FindComponentByClass<UCameraComponent>();
	if (!Camera) return;

	// Get Camera Location
	const FVector CameraWorldLocation = Camera->GetComponentLocation();

	// Get Actor Location/Rotation
	const FVector ActorLocation = ControlledCharacter->GetActorLocation();

	// Camera Offset relative to Actor BEFORE rotation
	const FVector CameraOffset = CameraWorldLocation - ActorLocation;

	// Calculate new relative position
	const FVector RotatedOffset = FRotationMatrix(FRotator(0.0f, TurnAngle, 0.0f)).TransformVector(CameraOffset);

	// Update Actor location to maintain camera's relative position
	const FVector NewActorLocation = CameraWorldLocation - RotatedOffset;
	ControlledCharacter->SetActorLocation(NewActorLocation, false, nullptr, ETeleportType::TeleportPhysics);

	UpdateCapsulePositionAndRotation();
}

void AVRPlayerController::GrabLeft(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}

void AVRPlayerController::ReleaseLeft(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}

void AVRPlayerController::GrabRight(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}

void AVRPlayerController::ReleaseRight(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}

void AVRPlayerController::ToggleLeftMenu(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}

void AVRPlayerController::ToggleRightMenu(const FInputActionValue& InputActionValue)
{
	if (!ControlledCharacter) return;
}
