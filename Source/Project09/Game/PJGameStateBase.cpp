// PJGameStateBase.cpp

#include "Game/PJGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/PJPlayerController.h"

void APJGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(PC) == true)
	{
		APJPlayerController* PJPC = Cast<APJPlayerController>(PC);
		if (IsValid(PJPC) == true)
		{
			const FString NotificationString = InNameString + TEXT(" has joined the game.");
			PJPC->PrintChatMessageString(NotificationString);
		}
	}
}
