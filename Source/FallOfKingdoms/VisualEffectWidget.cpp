// Fill out your copyright notice in the Description page of Project Settings.


#include "VisualEffectWidget.h"

void UVisualEffectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bLoopAnimation = true;
	PlayAnimation(GetVisualEffectAnimation(), 0, GetLoopAnimation() ? 0 : 1);
}