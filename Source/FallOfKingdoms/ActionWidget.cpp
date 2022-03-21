// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionWidget.h"
#include "Components/Image.h"

/*
void UActionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}
*/

void UActionWidget::StartActionProgress(float Duration)
{
	ActionDuration = Duration;
	ActionStartTime = GetWorld()->GetRealTimeSeconds();
	GetWorld()->GetTimerManager().SetTimer(ActionProgressTimer, this, &UActionWidget::UpdateActionProgress, 0.01f, true);
	UpdateActionProgress();
	ActionProgressImage->SetVisibility(ESlateVisibility::Visible);
}

void UActionWidget::StopActionProgress()
{
	ActionProgressImage->SetVisibility(ESlateVisibility::Hidden);
	GetWorld()->GetTimerManager().ClearTimer(ActionProgressTimer);
}

void UActionWidget::UpdateActionProgress()
{
	float _Value = (GetWorld()->GetRealTimeSeconds() - ActionStartTime) / ActionDuration;
	_Value > 1 ? _Value = 1 : _Value;
	UE_LOG(LogTemp, Warning, TEXT("%f"), _Value);

	GetActionProgressImage()->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Value"), _Value);
}