// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResponseWidget.generated.h"

class UButton;
class UTextBlock;
class UTalkWidget;
class ACharacterController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FResponseButtonDelegate, int, TalkID, int, ResponseID, int, ReturnTalkID, ACharacterController*, CharacterController);

UCLASS()
class FALLOFKINGDOMS_API UResponseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UButton* ResponseButton;
		UButton* GetResponseButton() const { return ResponseButton; }

	UPROPERTY(VisibleAnywhere, meta = (BindWidget))
		UTextBlock* ResponseTextBlock;
		UTextBlock* GetResponseTextBlock() const { return ResponseTextBlock; }

	UPROPERTY(VisibleAnywhere, Transient)
		int TalkID;
		int GetTalkID() const { return TalkID; }

	UPROPERTY(VisibleAnywhere, Transient)
		int ResponseID;
		int GetResponseID() const { return ResponseID; }

	UPROPERTY(VisibleAnywhere, Transient)
		int ReturnTalkID;
		int GetReturnTalkID() const { return ReturnTalkID; }

	UPROPERTY(VisibleAnywhere)
		FResponseButtonDelegate ResponseButtonDelegate;

	// METHODS
	UFUNCTION()
		void ResponseClicked();

	UFUNCTION()
		void SetResponseSettings(FText ResponseText, int NewTalkID, int NewResponseID, int NewReturnTalkID);
};
