// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionMessageWidget.generated.h"

class UTextBlock;

UENUM()
enum class EMessageType : uint8
{
	Default,
	Success,
	Warning,
	Error
};

UCLASS()
class FALLOFKINGDOMS_API UActionMessageWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		FLinearColor DefaultTextColor;
		FLinearColor GetDefaultTextColor() const { return DefaultTextColor; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		FLinearColor SuccessTextColor;
		FLinearColor GetSuccessTextColor() const { return SuccessTextColor; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		FLinearColor WarningTextColor;
		FLinearColor GetWarningTextColor() const { return WarningTextColor; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		FLinearColor ErrorTextColor;
		FLinearColor GetErrorTextColor() const { return ErrorTextColor; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float HideTextSpeed;
		float GetHideTextSpeed() const { return HideTextSpeed; }

	// PROPERTIES / COMPONENTS
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* MessageTextBox;
		UTextBlock* GetMessageTextBox() const { return MessageTextBox; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ShowMessageAnimation;
		UWidgetAnimation* GetShowMessageAnimation() const { return ShowMessageAnimation; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HideMessageAnimation;
		UWidgetAnimation* GetHideMessageAnimation() const { return HideMessageAnimation; }

	FTimerHandle HideMessageTimer;

	// METHODS
	UFUNCTION()
		void ShowMessage(FText Message, EMessageType MessageType);

	UFUNCTION()
		void HideMessage();
};
