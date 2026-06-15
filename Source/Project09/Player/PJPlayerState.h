// PJPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PJPlayerState.generated.h"

UCLASS()
class PROJECT09_API APJPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	APJPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	// 현재 시도 횟수 문자열 생성
	FString GetPlayerInfoString() const;

public:
	// 플레이어 표시 이름
	UPROPERTY(Replicated)
	FString PlayerNameString;

	// 현재 시도 횟수
	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	// 최대 시도 횟수
	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};