// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IdentifierWidget.generated.h"

class UProgressBar;
class UTextBlock;
// class UHealthComponent;

UCLASS()
class FALLOFKINGDOMS_API UIdentifierWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// PROPERTIES
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
		UProgressBar* ActorHealthBar;
		UProgressBar* GetActorHealthBar() const { return ActorHealthBar; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "Components")
		UTextBlock* ActorNameTextBlock;
		UTextBlock* GetActorNameTextBlock() const { return ActorNameTextBlock; }

	/*
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UHealthComponent* HealthComponent;
		UHealthComponent* GetHealthComponent() const { return HealthComponent; }
	*/


	// METHODS
	UFUNCTION()
		void SetActorName(FText NewName);

protected:
	virtual void NativeConstruct() override;
};
