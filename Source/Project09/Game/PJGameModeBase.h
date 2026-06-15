// PJGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PJGameModeBase.generated.h"

class APJPlayerController;

UCLASS()
class PROJECT09_API APJGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(APJPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(APJPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(APJPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TWeakObjectPtr<APJPlayerController>> AllPlayerControllers;
};
