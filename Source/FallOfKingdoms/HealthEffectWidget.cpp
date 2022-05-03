// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthEffectWidget.h"
#include "Components/Image.h"
#include "HealthEffect.h"

void UHealthEffectWidget::SetEffectImageSettings(UTexture2D* NewTexture, FLinearColor NewColor)
{
	GetEffectImage()->SetBrushFromTexture(NewTexture);
	GetEffectImage()->SetColorAndOpacity(NewColor);
}

void UHealthEffectWidget::SetHealthEffectReference(UHealthEffect* NewHealthEffect)
{
	HealthEffectReference = NewHealthEffect;

	GetEffectImage()->SetToolTipText(GetHealthEffectReference()->GetEffectDescription());
}
