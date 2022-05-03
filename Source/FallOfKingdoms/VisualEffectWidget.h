// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VisualEffectWidget.generated.h"

/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API UVisualEffectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* VisualEffectAnimation;
		UWidgetAnimation* GetVisualEffectAnimation() const { return VisualEffectAnimation; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bLoopAnimation;
		bool GetLoopAnimation() const { return bLoopAnimation; }
};
