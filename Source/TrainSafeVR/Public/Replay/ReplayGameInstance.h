// Copyright © 2024 Luis M. Infante \ Licensed under the GNU General Public License v3.0 (GPLv3).\ See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ReplayGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRAINSAFEVR_API UReplayGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UReplayGameInstance();

	UFUNCTION(BlueprintCallable, Category = "Replays")
	void StartRecording();

	UFUNCTION(BlueprintCallable, Category = "Replays")
	void StopRecording();

	UFUNCTION(BlueprintCallable, Category = "Replays")
	void StartReplay();

	UPROPERTY(EditDefaultsOnly, Category = "Replays")
	FString RecordingName;
	
	UPROPERTY(EditDefaultsOnly, Category = "Replays")
	FString FriendlyRecordingName;
};
