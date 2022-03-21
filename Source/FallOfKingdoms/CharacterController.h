// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterController.generated.h"

class UCharacterHUD;
class UActionMessageWidget;
class UUserWidget;
class APlayerCharacter;

UCLASS()
class FALLOFKINGDOMS_API ACharacterController : public APlayerController
{
	GENERATED_BODY()

public:
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Logic")
		APlayerCharacter* PlayerCharacter;
		APlayerCharacter* GetPlayerCharacter() const { return PlayerCharacter; }

	// METHODS
	UFUNCTION()
		void ShowMessage(FText Message, EMessageType MessageType);

	UFUNCTION()
		void AddWidgetEnablingMouse(UUserWidget* WidgetReference);

	UFUNCTION()
		void RemoveWidgetEnablingMouse(UUserWidget* WidgetReference);

	UFUNCTION()
		void UpdateMouseCursor(bool bVisibility);

	UFUNCTION()
		void UpdateIdentifiersVisibility(bool bVisibility);

protected:
	virtual void BeginPlay() override;

	// PROPERTIES
	UPROPERTY()
		TArray<UUserWidget*> WidgetEnablingMouse;

	
};
