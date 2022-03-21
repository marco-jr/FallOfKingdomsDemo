// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemObjectWidget.h"
#include "Components/Image.h"

void UItemObjectWidget::SetItemObjectImage(UTexture2D* NewImage)
{
	ItemObjectImage->SetBrushFromTexture(NewImage, true);
}