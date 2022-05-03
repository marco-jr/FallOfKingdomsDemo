// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../VisualEffectWidget.h"
#include "HungryVisualEffect.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API UHungryVisualEffect : public UVisualEffectWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* HungryImage;
		UImage* GetHungryEffectImage() const { return HungryImage; }
};
