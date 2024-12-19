// Copyright © 2024 Luis M. Infante
// Licensed under the GNU General Public License v3.0 (GPLv3).
// See the full license at https://www.gnu.org/licenses/gpl-3.0.html.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "VRCharacter.generated.h"


class AVRPlayerController;

UCLASS()
class TRAINSAFEVR_API AVRCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVRCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* APawn Overrides */
	virtual void PossessedBy(AController* NewController) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	void InitActorInfo();
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Player Controller")
	TObjectPtr<AVRPlayerController> PlayerController;
};
