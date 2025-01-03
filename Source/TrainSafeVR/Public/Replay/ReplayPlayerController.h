// Copyright © 2024 Luis M. Infante \ Licensed under the GNU General Public License v3.0 (GPLv3).\ See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ReplayPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TRAINSAFEVR_API AReplayPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Replays")
	void RestartRecording();
};
