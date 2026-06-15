// PJGameModeBase.cpp

#include "Game/PJGameModeBase.h"

#include "Game/PJGameStateBase.h"
#include "Player/PJPlayerController.h"
#include "Player/PJPlayerState.h"
#include "EngineUtils.h"

void APJGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	APJPlayerController* PJPlayerController = Cast<APJPlayerController>(NewPlayer);
	if (IsValid(PJPlayerController) == true)
	{
		PJPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server"));

		AllPlayerControllers.Add(PJPlayerController);

		APJPlayerState* PJPS = PJPlayerController->GetPlayerState<APJPlayerState>();
		if (IsValid(PJPS) == true)
		{
			PJPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		APJGameStateBase* PJGameStateBase = GetGameState<APJGameStateBase>();
		if (IsValid(PJGameStateBase) == true)
		{
			PJGameStateBase->MulticastRPCBroadcastLoginMessage(PJPS->PlayerNameString);
		}
	}
}

FString APJGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool APJGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString APJGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void APJGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
	UE_LOG(LogTemp, Error, TEXT("%s"), *SecretNumberString);
}

void APJGameModeBase::PrintChatMessageString(APJPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<APJPlayerController> It(GetWorld()); It; ++It)
		{
			APJPlayerController* PJPlayerController = *It;
			if (IsValid(PJPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				PJPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<APJPlayerController> It(GetWorld()); It; ++It)
		{
			APJPlayerController* PJPlayerController = *It;
			if (IsValid(PJPlayerController) == true)
			{
				PJPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void APJGameModeBase::IncreaseGuessCount(APJPlayerController* InChattingPlayerController)
{
	APJPlayerState* PJPS = InChattingPlayerController->GetPlayerState<APJPlayerState>();
	if (IsValid(PJPS) == true)
	{
		PJPS->CurrentGuessCount++;
	}
}

void APJGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& PJPlayerController : AllPlayerControllers)
	{
		APJPlayerState* PJPS = PJPlayerController->GetPlayerState<APJPlayerState>();
		if (IsValid(PJPS) == true)
		{
			PJPS->CurrentGuessCount = 0;
		}
	}
}

void APJGameModeBase::JudgeGame(APJPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		APJPlayerState* PJPS = InChattingPlayerController->GetPlayerState<APJPlayerState>();
		for (const auto& PJPlayerController : AllPlayerControllers)
		{
			if (IsValid(PJPS) == true)
			{
				FString CombinedMessageString = PJPS->PlayerNameString + TEXT(" has won the game.");
				PJPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& APJPlayerController : AllPlayerControllers)
		{
			APJPlayerState* PJPS = APJPlayerController->GetPlayerState<APJPlayerState>();
			if (IsValid(PJPS) == true)
			{
				if (PJPS->CurrentGuessCount < PJPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& PJPlayerController : AllPlayerControllers)
			{
				PJPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}

