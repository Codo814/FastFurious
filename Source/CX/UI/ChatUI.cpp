// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatUI.h"
#include "CXPlayerController.h"

void UChatUI::NativeConstruct()
{
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UChatUI::OnChatInputTextCommiteed) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &UChatUI::OnChatInputTextCommiteed);
	}
	Super::NativeConstruct();
}

void UChatUI::NativeDestruct()
{
	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &UChatUI::OnChatInputTextCommiteed) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &UChatUI::OnChatInputTextCommiteed);
	}
	Super::NativeDestruct();
}

void UChatUI::OnNewChatMessage(const FString& Sender, const FString& Message)
{
}

void UChatUI::OnChatInputTextCommiteed(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		if (Text.IsEmpty() == false)
		{
			FString ChatMessage = Text.ToString();
			EditableTextBox_ChatInput->SetText(FText::GetEmpty());
			if (ACXPlayerController* PC = GetOwningPlayer<ACXPlayerController>())
			{
				PC->ClientToServerChatMessage(ChatMessage);
			}
		}
	}
}