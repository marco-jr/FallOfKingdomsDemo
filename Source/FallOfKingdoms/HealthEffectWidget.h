// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthEffectWidget.generated.h"

class UImage;
class UTexture2D;
class UHealthEffect;
/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API UHealthEffectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Settings")
		UHealthEffect* HealthEffectReference;
		UHealthEffect* GetHealthEffectReference() const { return HealthEffectReference; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* EffectImage;
		UImage* GetEffectImage() const { return EffectImage; }

	// METHODS
	UFUNCTION()
		void SetEffectImageSettings(UTexture2D* NewTexture, FLinearColor NewColor);

	UFUNCTION()
		void SetHealthEffectReference(UHealthEffect* NewHealthEffect);
};
