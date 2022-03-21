// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HandWidget.generated.h"

class UBorder;
class UHandComponent;
class UItemObjectWidget;
class UBackgroundBlur;

UCLASS()
class FALLOFKINGDOMS_API UHandWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES
	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "Components")
		UBackgroundBlur* RightHandRoot;

	UPROPERTY(EditAnywhere, meta = (BindWidget), Category = "Components")
		UBackgroundBlur* LeftHandRoot;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (BindWidget))
		UBorder* RightHandBorder;

	UPROPERTY(EditAnywhere, Category = "Components", meta = (BindWidget))
		UBorder* LeftHandBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
		UBorder* HandBorder;

	UPROPERTY(BlueprintReadOnly, Category = "Components")
		UBackgroundBlur* RootBackground;

	UPROPERTY(VisibleAnywhere, Transient, Category = "Components")
		UHandComponent* OwnerHand;
		UHandComponent* GetOwnerHand() const { return OwnerHand; }

	UPROPERTY(VisibleAnywhere, Transient, Category = "Components")
		UItemObjectWidget* ItemObjectWidget;

	// METHODS
	UFUNCTION()
		void SetOwnerHand(UHandComponent* HandReference);

	UFUNCTION()
		void AddItemObject(UItemObjectWidget* ItemObjectWidgetReference);

	UFUNCTION()
		void RemoveItemObject();
};