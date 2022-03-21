// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkWidget.h"
#include "Components/TextBlock.h"
#include "ResponseWidget.h"
#include "TalkComponent.h"
#include "Components/VerticalBox.h"
#include "CharacterController.h"
#include "Components/Button.h"
#include "CharacterIdentity.h"
#include "ActionMessageWidget.h"
#include "Components/Border.h"
#include "Components/BackgroundBlur.h"

void UTalkWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UTalkWidget::UpdateTalk(int TalkID, ACharacterController* ControllerReference, bool bAnimateText)
{
	if (TalkComponent != nullptr)
	{
		ClearResponses();
		PlayAnimation(ShowTextAnim);

		if (TalkID >= 0)
		{
			if (TalkComponent->TalkSettings[TalkID].Responses.Num() > 0)
			{
				for (FResponse _CurrentResponse : TalkComponent->TalkSettings[TalkID].Responses)
				{
					CreateResponse(TalkID, _CurrentResponse.ResponseText, _CurrentResponse.ReturnTalkID, ControllerReference);
				}
			}
			
			CreateResponse(TalkID, FText::FromString("Despedir-se"), -1, ControllerReference);
			
			if (bAnimateText)
			{
				CurrentText = UpdateLastChar(TalkComponent->TalkSettings[TalkID].TalkText);
				CurrentTextIndex = 1;
				GetWorld()->GetTimerManager().SetTimer(AnimateTextHandle, this, &UTalkWidget::AnimateText, GetLetterShowSpeed(), true);
				AnimateText();
			}
			else
			{
				CurrentText = UpdateLastChar(TalkComponent->TalkSettings[TalkID].TalkText);
				NPCResponseTextBlock->SetText(CurrentText);
			}
		}
		else if (TalkID == -1)
		{
			CurrentText = UpdateLastChar(TalkComponent->GetRandomFarewellText());		
			NPCResponseTextBlock->SetText(CurrentText);
			TalkComponent->EndTalk();
		}

	}
}

void UTalkWidget::ClearResponses()
{
	for (UResponseWidget* _CurrentResponseWidget : CreatedResponseWidgets)
	{
		_CurrentResponseWidget->RemoveFromParent();
		_CurrentResponseWidget->RemoveFromViewport();
	}

	CreatedResponseWidgets.Empty();
}

void UTalkWidget::CreateResponse(int TalkID, FText ResponseText, int ReturnTalkID, ACharacterController* ControllerReference)
{
	UResponseWidget* _CurrentResponseWidget = CreateWidget<UResponseWidget>(ControllerReference, ResponseWidgetClass);
	int _CreatedResponseIndex = CreatedResponseWidgets.Add(_CurrentResponseWidget);
	_CurrentResponseWidget->SetResponseSettings(ResponseText, TalkID, _CreatedResponseIndex, ReturnTalkID);
	ResponsesVerticalBox->AddChildToVerticalBox(_CurrentResponseWidget);
	_CurrentResponseWidget->ResponseButtonDelegate.AddDynamic(this, &UTalkWidget::ResponseClicked);

}

void UTalkWidget::ResponseClicked(int TalkID, int ResponseID, int ReturnTalkID, ACharacterController* CharacterController)
{
	UpdateTalk(ReturnTalkID, CharacterController);
	TalkComponent->ResponseDelegate.Broadcast(TalkID, ResponseID, ReturnTalkID);
}

void UTalkWidget::SetTalkComponent(UTalkComponent* TalkReference)
{
	TalkComponent = TalkReference;
	if (UCharacterIdentity* _CharacterIdentity = TalkComponent->GetOwner()->FindComponentByClass<UCharacterIdentity>())
	{
		NPCNameTextBlock->SetText(_CharacterIdentity->GetCharacterName());
	}
}

void UTalkWidget::AnimateText()
{
	FText _CurrentText = FText::FromString(CurrentText.ToString().Left(CurrentTextIndex));
	NPCResponseTextBlock->SetText(_CurrentText);
	CurrentTextIndex++;

	if (CurrentText.ToString().Len() == CurrentTextIndex - 1)
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimateTextHandle);
	}
}

void UTalkWidget::ShowTalkAnimation()
{
	PlayAnimation(ShowTalkAnim);
}

void UTalkWidget::SwitchResponsesVisibility()
{
	ESlateVisibility _NewVisibility = GetRootResponsesWidget()->GetVisibility() == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	GetRootResponsesWidget()->SetVisibility(_NewVisibility);
}

FText UTalkWidget::UpdateLastChar(FText SourceText)
{
	FText _ResultText = SourceText;
	if (!_ResultText.ToString().EndsWith(".") && !_ResultText.ToString().EndsWith("?") && !_ResultText.ToString().EndsWith("!"))
	{
		_ResultText = FText::FromString(_ResultText.ToString() + ".");
		return _ResultText;
	}
	return _ResultText;
}
