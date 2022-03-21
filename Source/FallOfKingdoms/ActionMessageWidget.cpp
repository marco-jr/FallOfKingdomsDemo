// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionMessageWidget.h"
#include "Components/TextBlock.h"
#include "Styling/SlateColor.h"

void UActionMessageWidget::ShowMessage(FText Message, EMessageType MessageType)
{
	FLinearColor _TextColor;

	switch (MessageType)
	{
	case EMessageType::Default:
		_TextColor = GetDefaultTextColor();
		break;
	case EMessageType::Success:
		_TextColor = GetSuccessTextColor();
		break;
	case EMessageType::Warning:
		_TextColor = GetWarningTextColor();
		break;
	case EMessageType::Error:
		_TextColor = GetErrorTextColor();
		break;
	default:
		_TextColor = GetDefaultTextColor();
		break;
	}
	GetMessageTextBox()->SetColorAndOpacity(FSlateColor(_TextColor));
	PlayAnimation(ShowMessageAnimation, 0, 1, EUMGSequencePlayMode::Forward, GetHideTextSpeed());
	if (!Message.ToString().EndsWith(".") && !Message.ToString().EndsWith("?") && !Message.ToString().EndsWith("!"))
	{
		Message = FText::FromString(Message.ToString() + ".");
	}
	GetMessageTextBox()->SetText(Message);
	FString _ReducedMessage = Message.ToString().Replace(TEXT(" "), TEXT(""));
	float _MessageDuration = float(_ReducedMessage.Len() * 0.25f);

	if (_MessageDuration > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(HideMessageTimer, this, &UActionMessageWidget::HideMessage, _MessageDuration, false);
	}
}

void UActionMessageWidget::HideMessage()
{
	PlayAnimation(HideMessageAnimation);
}