// Copyright © 2024 Luis M. Infante
// Licensed under the GNU General Public License v3.0 (GPLv3).
// See the full license at https://www.gnu.org/licenses/gpl-3.0.html.


#include "Character/VRCharacter.h"
#include "Player/VRPlayerController.h"

// Sets default values
AVRCharacter::AVRCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVRCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVRCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	InitActorInfo();
}

void AVRCharacter::InitActorInfo()
{
	PlayerController = Cast<AVRPlayerController>(GetController());
}

