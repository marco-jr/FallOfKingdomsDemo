// Fill out your copyright notice in the Description page of Project Settings.


#include "IdentifierWidget.h"
#include "Components/TextBlock.h"

void UIdentifierWidget::SetActorName(FText NewName)
{
	GetActorNameTextBlock()->SetText(NewName);
}

void UIdentifierWidget::NativeConstruct()
{
	Super::NativeConstruct();
	// TODO: BIND HEALTH CHANGED TO UPDATE BAR
}
