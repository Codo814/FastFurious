// Copyright Epic Games, Inc. All Rights Reserved.


#include "CXUI.h"
#include "Components/EditableTextBox.h"
#include "GameState/CXGameState.h"
#include "UI/ChatUI.h"

void UCXUI::OnNewChatMessage(const FString& InSender, const FString& InMessage)
{
	UI_Chat->OnNewChatMessage(InSender, InMessage);
}

void UCXUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	const ACXGameState* CXGameState = GetWorld() ? GetWorld()->GetGameState<ACXGameState>() : nullptr;
	if (!IsValid(CXGameState))
	{
		return;
	}

	if (UEditableTextBox* RemainingTimeText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("RemainingTimeText"))))
	{
		RemainingTimeText->SetText(FText::AsNumber(CXGameState->GetRemainingTime()));
	}

	if (UEditableTextBox* GameResultText = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("GameResultText"))))
	{
		const bool bGameFinished = CXGameState->GetCurrentGameState() == EGameState::Finished;
		GameResultText->SetVisibility(bGameFinished ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

		if (bGameFinished)
		{
			GameResultText->SetText(FText::FromString(CXGameState->IsWinner(GetOwningPlayerState()) ? TEXT("You win!") : TEXT("You lose")));
		}
	}
}

void UCXUI::UpdateSpeed(float NewSpeed)
{
	// format the speed to KPH or MPH
	float FormattedSpeed = FMath::Abs(NewSpeed) * (bIsMPH ? 0.022f : 0.036f);

	// call the Blueprint handler
	OnSpeedUpdate(FormattedSpeed);
}

void UCXUI::UpdateGear(int32 NewGear)
{
	// call the Blueprint handler
	OnGearUpdate(NewGear);
}
