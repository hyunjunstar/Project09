// PJPlayerController.cpp

#include "Player/PJPlayerController.h"
#include "UI/PJChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Project09.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/PJGameModeBase.h"
#include "PJPlayerState.h"
#include "Net/UnrealNetwork.h"

APJPlayerController::APJPlayerController()
{
	bReplicates = true;
}

void APJPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UPJChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void APJPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		ServerRPCPrintChatMessageString(InChatMessageString);
	}
}

void APJPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	Project09FunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void APJPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void APJPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	// 서버 GameMode로 채팅 처리 위임
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		APJGameModeBase* PJGM = Cast<APJGameModeBase>(GM);
		if (IsValid(PJGM) == true)
		{
			PJGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

void APJPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}