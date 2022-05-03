// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InteractableWidget.generated.h"

class UTextBlock;
class UImage;
class UTexture2D;

UCLASS()
class FALLOFKINGDOMS_API UInteractableWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetActionText(FText NewActionText);
	void SetActionImage(UTexture2D* NewActionImage);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UTextBlock* ActionTextBlock;
		UTextBlock* GetActionTextBlock() const { return ActionTextBlock; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* InteractImageBox;
		UImage* GetInteractImageBox() const { return InteractImageBox; }
};
