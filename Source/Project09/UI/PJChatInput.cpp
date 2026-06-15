// PJChatInput.cpp

#include "UI/PJChatInput.h"

#include "Components/EditableTextBox.h"
#include "Player/PJPlayerController.h"

void UPJChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	// 입력 박스 유효성 확인
	if (EditableTextBox_ChatInput == nullptr)
	{
		return;
	}

	// Enter 입력 이벤트 연결
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UPJChatInput::NativeDestruct()
{
	// Enter 입력 이벤트 해제
	if (EditableTextBox_ChatInput != nullptr
		&& EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}

	Super::NativeDestruct();
}

void UPJChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	// Enter 입력인지 확인
	if (CommitMethod != ETextCommit::OnEnter)
	{
		return;
	}

	APJPlayerController* OwningPJPlayerController = GetOwningPlayer<APJPlayerController>();
	if (IsValid(OwningPJPlayerController) == false)
	{
		return;
	}

	// PlayerController로 채팅 문자열 전달
	OwningPJPlayerController->SetChatMessageString(Text.ToString());

	// 입력창 비우기
	if (EditableTextBox_ChatInput != nullptr)
	{
		EditableTextBox_ChatInput->SetText(FText());
	}
}
