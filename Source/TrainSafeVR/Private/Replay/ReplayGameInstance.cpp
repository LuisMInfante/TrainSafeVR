// Copyright © 2024 Luis M. Infante \ Licensed under the GNU General Public License v3.0 (GPLv3).\ See the full license at https://www.gnu.org/licenses/gpl-3.0.html.


#include "Replay/ReplayGameInstance.h"

UReplayGameInstance::UReplayGameInstance()
{
	RecordingName = "MyReplay";
	FriendlyRecordingName = "My Replay";
}

void UReplayGameInstance::StartRecording()
{
	StartRecordingReplay(RecordingName, FriendlyRecordingName);
}

void UReplayGameInstance::StopRecording()
{
	StopRecordingReplay();
}

void UReplayGameInstance::StartReplay()
{
	PlayReplay(RecordingName, nullptr);
}
