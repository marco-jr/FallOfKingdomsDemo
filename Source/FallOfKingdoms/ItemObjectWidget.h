// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemObjectWidget.generated.h"

class UImage;
class UTexture2D;

UCLASS()
class FALLOFKINGDOMS_API UItemObjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* ItemObjectImage;

	// METHODS
	UFUNCTION()
		void SetItemObjectImage(UTexture2D* NewImage);
};
