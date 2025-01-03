// Copyright © 2024 Luis M. Infante \ Licensed under the GNU General Public License v3.0 (GPLv3).\ See the full license at https://www.gnu.org/licenses/gpl-3.0.html.


#include "Replay/ReplayPlayerController.h"

#include "Engine/DemoNetDriver.h"
#include "Engine/World.h"

void AReplayPlayerController::RestartRecording()
{
	if (UWorld* World = GetWorld())
	{
		if (UDemoNetDriver* DemoDriver = World->GetDemoNetDriver())
		{
			DemoDriver->GotoTimeInSeconds((0.0f));
		}
	}
}
