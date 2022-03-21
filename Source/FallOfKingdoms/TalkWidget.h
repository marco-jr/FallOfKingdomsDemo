// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalkWidget.generated.h"

class UBorder;
class UTextBlock;
class UResponseWidget;
class UWidgetAnimation;
class UTalkComponent;
class UVerticalBox;
class ACharacterController;
class UBackgroundBlur;

UCLASS()
class FALLOFKINGDOMS_API UTalkWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UResponseWidget> ResponseWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Settings")
		int LetterSpace;
		int GetLetterSpace() const { return LetterSpace; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float LetterShowSpeed;
		float GetLetterShowSpeed() const { return LetterShowSpeed; }

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Components")
		UBackgroundBlur* RootResponsesWidget;
		UBackgroundBlur* GetRootResponsesWidget() const { return RootResponsesWidget; }

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Components")
		UBorder* PlayerResponseBorder;
		UBorder* GetPlayerResponseBorder() const { return PlayerResponseBorder; }

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Components")
		UTextBlock* NPCResponseTextBlock;
		UTextBlock* GetNPCResponseTextBlock() const { return NPCResponseTextBlock; }

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Components")
		UTextBlock* NPCNameTextBlock;
		UTextBlock* GetNPCNameTextBlock() const { return NPCNameTextBlock; }

	UPROPERTY(VisibleAnywhere, meta = (BindWidget), Category = "Components")
		UVerticalBox* ResponsesVerticalBox;
		UVerticalBox* GetResponsesVerticalBox() const { return ResponsesVerticalBox; }

	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ShowTextAnim;
		UWidgetAnimation* GetShowTextAnim() const { return ShowTextAnim; }

	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ShowTalkAnim;
		UWidgetAnimation* GetShowTalkAnim() const { return ShowTalkAnim; }

	UPROPERTY(VisibleAnywhere, Transient, Category = "Logic")
		UTalkComponent* TalkComponent;
		UTalkComponent* GetTalkComponent() const { return TalkComponent; }

	UPROPERTY()
		TArray<UResponseWidget*> CreatedResponseWidgets;

	int CurrentTextIndex;
	FText CurrentText;
	FTimerHandle AnimateTextHandle;

	// METHODS
	UFUNCTION()
		void UpdateTalk(int TalkID, ACharacterController* ControllerReference, bool bAnimateText = false);

	UFUNCTION()
		void ClearResponses();

	UFUNCTION()
		void CreateResponse(int TalkID, FText ResponseText, int ReturnTalkID, ACharacterController* ControllerReference);

	UFUNCTION()
		void ResponseClicked(int TalkID, int ResponseID, int ReturnTalkID, ACharacterController* CharacterController);

	UFUNCTION()
		void SetTalkComponent(UTalkComponent* TalkReference);

	UFUNCTION()
		void AnimateText();

	UFUNCTION()
		void ShowTalkAnimation();

	UFUNCTION()
		void SwitchResponsesVisibility();

	UFUNCTION()
		FText UpdateLastChar(FText SourceText);
};
