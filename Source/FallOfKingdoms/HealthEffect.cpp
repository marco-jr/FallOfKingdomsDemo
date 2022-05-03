// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthEffect.h"
#include "HealthEffectWidget.h"
#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthEffect::UHealthEffect()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	EffectDuration = 0;
}

void UHealthEffect::StartEffect(UHealthComponent* HealthComponentReference)
{
	if (HealthComponentReference != nullptr)
	{
		HealthComponent = HealthComponentReference;
		EffectStarted();
		if (GetEffectDuration() > 0)
		{
			GetWorld()->GetTimerManager().SetTimer(EffectTimer, this, &UHealthEffect::StopEffect, GetEffectDuration(), true);
		}
	}
}

void UHealthEffect::StopEffect()
{
	HealthComponent->RemoveHealthEffect(this);
}

void UHealthEffect::EffectStarted()
{
}

void UHealthEffect::EffectStoped()
{
}

// Called when the game starts
void UHealthEffect::BeginPlay()
{
	Super::BeginPlay();

}