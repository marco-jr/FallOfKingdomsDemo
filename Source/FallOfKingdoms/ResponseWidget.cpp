// Fill out your copyright notice in the Description page of Project Settings.


#include "ResponseWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "CharacterController.h"

void UResponseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ResponseButton->OnClicked.AddDynamic(this, &UResponseWidget::ResponseClicked);
}

void UResponseWidget::ResponseClicked()
{
	ACharacterController* _CharacterController = Cast<ACharacterController>(GetWorld()->GetFirstPlayerController());
	ResponseButtonDelegate.Broadcast(TalkID, ResponseID, ReturnTalkID, _CharacterController);
}

void UResponseWidget::SetResponseSettings(FText ResponseText, int NewTalkID, int NewResponseID, int NewReturnTalkID)
{
	TalkID = NewTalkID;
	ResponseID = NewResponseID;
	ReturnTalkID = NewReturnTalkID;

	ResponseTextBlock->SetText(ResponseText);
}