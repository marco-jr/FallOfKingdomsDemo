// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterController.h"
#include "CharacterHUD.h"
#include "PlayerCharacter.h"
#include "ActionMessageWidget.h"

void ACharacterController::AddWidgetEnablingMouse(UUserWidget* WidgetReference)
{
	WidgetEnablingMouse.AddUnique(WidgetReference);

	UpdateMouseCursor(true);
}

void ACharacterController::RemoveWidgetEnablingMouse(UUserWidget* WidgetReference)
{
	TArray<UUserWidget*> _WidgetsEnablingMouse = WidgetEnablingMouse;

	if (WidgetEnablingMouse.Contains(WidgetReference))
	{
		WidgetEnablingMouse.Remove(WidgetReference);
	}
	if (WidgetEnablingMouse.Num() == 0)
	{
		UpdateMouseCursor(false);
	}
}

void ACharacterController::UpdateMouseCursor(bool bVisibility)
{
	if (bVisibility)
	{
		SetShowMouseCursor(true);
		FInputModeGameAndUI _InputModeUI;
		SetInputMode(_InputModeUI);
	}
	else
	{
		SetShowMouseCursor(false);
		FInputModeGameOnly _InputGameOnly;
		SetInputMode(_InputGameOnly);
	}
}

void ACharacterController::UpdateIdentifiersVisibility(bool bVisibility)
{
	if (PlayerCharacter != nullptr)
	{
		PlayerCharacter->UpdateIdentifiersVisibility(bVisibility);
	}
}

void ACharacterController::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(GetCharacter());

	WidgetEnablingMouse.Init(nullptr, 0);
}

void ACharacterController::ShowMessage(FText Message, EMessageType MessageType)
{
	GetPlayerCharacter()->ShowMessage(Message, MessageType);
}