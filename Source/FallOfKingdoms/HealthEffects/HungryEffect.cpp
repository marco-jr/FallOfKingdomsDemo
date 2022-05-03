// Fill out your copyright notice in the Description page of Project Settings.


#include "../HealthEffects/HungryEffect.h"
#include "../HealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

UHungryEffect::UHungryEffect()
{
	EffectDescription = FText::FromString("Você está com fome, é melhor comer algo logo! Redução de qualidade de vida aumentado e visão comprometida.");

	static ConstructorHelpers::FObjectFinder<USoundCue> _HungrySoundAsset(TEXT("SoundCue'/Game/SFX/SFX_StomachGrowling_Cue.SFX_StomachGrowling_Cue'"));
	if (_HungrySoundAsset.Succeeded())
	{
		StomachSound = _HungrySoundAsset.Object;
	}
}

void UHungryEffect::EffectStarted()
{
	HungryEffect();
	GetWorld()->GetTimerManager().SetTimer(HungryHandle, this, &UHungryEffect::HungryEffect, 10, true);
}

void UHungryEffect::HungryEffect()
{
	if (GetHealthComponent() != nullptr)
	{
		float _EffectIntensity = (650 - GetHealthComponent()->GetCalories()) / 500;
		if (FMath::RandRange(0, 100) < (_EffectIntensity * 100) && GetStomachSound() != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), GetStomachSound(), GetOwner()->GetActorLocation());
		}
		GetHealthComponent()->ChangeHealthByValue(EHealthType::HealthQuality, -0.054);
	}
}
