// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InteractableComponent.generated.h"

class UHandComponent;
class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInteractableDelegate, UHandComponent*, HandReference);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UInteractableComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractableComponent();

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		FText InteractionText;
		FText GetInteractionText() const { return InteractionText; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UTexture2D* InteractionImage;
		UTexture2D* GetInteractionImage() const { return InteractionImage; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bHandFreeNeeded;
		bool GetHandFreeNeeded() const { return bHandFreeNeeded; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bAnimateInteraction;
		bool GetAnimateInteraction() const { return bAnimateInteraction; }
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FInteractableDelegate InteractDelegate;

	// METHODS
	UFUNCTION()
		void Interacted(UHandComponent* HandReference);

	UFUNCTION()
		void UpdateCollision(bool bCollides = true);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
};
