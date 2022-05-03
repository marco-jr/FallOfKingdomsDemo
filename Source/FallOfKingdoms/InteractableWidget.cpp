// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UInteractableWidget::SetActionText(FText NewActionText)
{
	GetActionTextBlock()->SetText(NewActionText);
}

void UInteractableWidget::SetActionImage(UTexture2D* NewActionImage)
{
	if (NewActionImage != nullptr)
	{
		GetInteractImageBox()->SetBrushFromTexture(NewActionImage);
	}
}
