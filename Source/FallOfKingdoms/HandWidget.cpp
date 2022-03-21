#include "HandWidget.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "HandWidget.h"
#include "Components/Border.h"
#include "HandComponent.h"
#include "ItemObjectWidget.h"
#include "Components/BackgroundBlur.h"

void UHandWidget::SetOwnerHand(UHandComponent* HandReference)
{
	if (HandReference != nullptr)
	{
		OwnerHand = HandReference;
		HandBorder = OwnerHand->GetIsMainHand() ? RightHandBorder : LeftHandBorder;
		RootBackground = OwnerHand->GetIsMainHand() ? RightHandRoot : LeftHandRoot;
		RootBackground->SetVisibility(ESlateVisibility::Visible);

	}
}

void UHandWidget::AddItemObject(UItemObjectWidget* ItemObjectWidgetReference)
{
	ItemObjectWidget = ItemObjectWidgetReference;
	HandBorder->AddChild(ItemObjectWidgetReference);
}

void UHandWidget::RemoveItemObject()
{
	if (ItemObjectWidget)
	{
		HandBorder->RemoveChildAt(0);
		ItemObjectWidget = nullptr;
	}
}
