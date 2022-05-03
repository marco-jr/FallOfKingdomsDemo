// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HealthEffect.h"
#include "HungryEffect.generated.h"

class USoundBase;
/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API UHungryEffect : public UHealthEffect
{
	GENERATED_BODY()

public:
	UHungryEffect();

protected:
	virtual void EffectStarted() override;

	// PROPERTIES
	FTimerHandle HungryHandle;

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		USoundBase* StomachSound;
		USoundBase* GetStomachSound() const { return StomachSound; }

	// METHODS
	UFUNCTION()
		void HungryEffect();
	
};
