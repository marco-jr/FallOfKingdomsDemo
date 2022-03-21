// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidget.h"
#include "Components/TextBlock.h"

void UInteractableWidget::SetActionText(FText NewActionText)
{
	ActionTextBlock->SetText(NewActionText);
}