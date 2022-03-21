// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionWidget.generated.h"

class UImage;

UCLASS()
class FALLOFKINGDOMS_API UActionWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* ActionProgressImage;
		UImage* GetActionProgressImage() const { return ActionProgressImage; }

	FTimerHandle ActionProgressTimer;
	float ActionStartTime;
	float ActionDuration;

	UFUNCTION()
		void StartActionProgress(float Duration);

	UFUNCTION()
		void StopActionProgress();

	UFUNCTION()
		void UpdateActionProgress();
};
