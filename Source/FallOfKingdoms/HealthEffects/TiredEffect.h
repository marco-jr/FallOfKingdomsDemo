// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../HealthEffect.h"
#include "TiredEffect.generated.h"

class UHealthComponent;
/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API UTiredEffect : public UHealthEffect
{
	GENERATED_BODY()

public:
	UTiredEffect();

	virtual void EffectStarted() override;

	virtual void EffectStoped() override;
};