// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "HealthEffect.h"
#include "HealthEffects/TiredEffect.h"
#include "HealthEffects/DehydratedEffect.h"
#include "HealthEffects/HungryEffect.h"
#include "HealthWidget.h"
#include "PlayerCharacter.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bConsumeHealth = true;
	HealthConsumptionDelay = 10;
	HealthQuality = 65;
	Health = HealthQuality;
	Calories = 1500;
	Hidratation = 1500;
	Stamina = 100;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(GetOwner()))
	{
		if (GetHealthWidgetClass() != nullptr)
		{
			HealthWidget = CreateWidget<UHealthWidget>(GetWorld()->GetFirstPlayerController(), GetHealthWidgetClass());

			for (EHealthType _Current : TEnumRange<EHealthType>())
			{
				UpdateHealthWidget(_Current);
			}
			HealthWidget->AddToViewport(1);
		}
	}

	if (GetConsumeHealth())
	{
		ConsumeHealth();
		GetWorld()->GetTimerManager().SetTimer(ConsumeHealthTimer, this, &UHealthComponent::ConsumeHealth, GetHealthConsumptionDelay(), true);
	}
}

bool UHealthComponent::VerifyDehydration()
{
	if (GetHidratation() < 500 && GetHealthEffectByClass(UDehydratedEffect::StaticClass()) == nullptr)
	{
		AddHealthEffect(UDehydratedEffect::StaticClass());
		GetHealthWidget()->SetThirsty(true);
	}
	else if (GetHidratation() >= 500)
	{
		if (GetHealthEffectByClass(UDehydratedEffect::StaticClass()) != nullptr)
		{
			RemoveHealthEffect(GetHealthEffectByClass(UDehydratedEffect::StaticClass()));
			GetHealthWidget()->SetThirsty(false);
		}
	}
	return GetHidratation() < 500 ? true : false;
}

bool UHealthComponent::VerifyHungry()
{
	if (GetCalories() < 500 && GetHealthEffectByClass(UHungryEffect::StaticClass()) == nullptr)
	{
		AddHealthEffect(UHungryEffect::StaticClass());
		GetHealthWidget()->SetHungry(true);
	}
	else if (GetCalories() >= 500)
	{
		if (GetHealthEffectByClass(UHungryEffect::StaticClass()) != nullptr)
		{
			RemoveHealthEffect(GetHealthEffectByClass(UHungryEffect::StaticClass()));
			GetHealthWidget()->SetHungry(false);
		}
	}
	return GetCalories() < 500 ? true : false;
}

void UHealthComponent::ConsumeHealth()
{
	ChangeHealthByValue(EHealthType::Calories, -1.35f);
	ChangeHealthByValue(EHealthType::Hidratation, -1.85f);
	ChangeHealthByValue(EHealthType::HealthQuality, -0.018f);
	ChangeHealthByValue(EHealthType::Stamina, -0.055f);

	if (GetHealthEffectByClass(UTiredEffect::StaticClass()) == nullptr)
	{
		float _AddHealthValue = 1; // + Skills->Vitality;
		ChangeHealthByValue(EHealthType::Health, _AddHealthValue);
	}
}

void UHealthComponent::UpdateHealthWidget(EHealthType HealthType)
{
	if (GetHealthWidget() != nullptr)
	{
		float _Value;
		switch (HealthType)
		{
		case EHealthType::HealthQuality:
			_Value = GetHealthQuality() / 100;
			break;
		case EHealthType::Health:
			_Value = GetHealth() / FGenericPlatformMath::TruncToFloat(GetHealthQuality());
			break;
		case EHealthType::Calories:
			_Value = GetCalories() / 5000;
			_Value = _Value > 1 ? 1 : _Value;
			break;
		case EHealthType::Hidratation:
			_Value = GetHidratation() / 5000;
			_Value = _Value > 1 ? 1 : _Value;
			break;
		case EHealthType::Stamina:
			_Value = GetStamina() / 100;
			break;
		default:
			break;
		}

		GetHealthWidget()->UpdateHealth(HealthType, _Value);
	}
}

void UHealthComponent::ChangeHealthByValue(EHealthType HealthType, float Value)
{
	switch (HealthType)
	{
	case EHealthType::HealthQuality:
		HealthQuality += Value;
		HealthQuality = GetHealthQuality() > 100 ? 100 : GetHealthQuality();
		HealthQuality = GetHealthQuality() < 0 ? 0 : GetHealthQuality();
		Health = GetHealth() > GetHealthQuality() ? GetHealthQuality()  : GetHealth();
		if (GetHealthQuality() <= 0)
		{
			ZeroHealth();
		}
		break;
	case EHealthType::Health:
		Health += Value;
		Health = GetHealth() > GetHealthQuality() ? GetHealthQuality() : GetHealth();
		Health = GetHealth() < 0 ? 0 : GetHealth();
		if (GetHealth() <= 0)
		{
			ZeroHealth();
		}
		break;
	case EHealthType::Calories:
		Calories += Value;
		Calories = GetCalories() < 0 ? 0 : GetCalories();
		VerifyHungry();
		break;
	case EHealthType::Hidratation:
		Hidratation += Value;
		Hidratation = GetHidratation() < 0 ? 0 : GetHidratation();
		VerifyDehydration();
		break;
	case EHealthType::Stamina:
		Stamina += Value;
		Stamina = GetStamina() > 100 ? 100 : GetStamina();
		Stamina = GetStamina() < 0 ? 0 : GetStamina();
		if (Stamina < 40 && GetHealthEffectByClass(UTiredEffect::StaticClass()) == nullptr)
		{
			AddHealthEffect(UTiredEffect::StaticClass());
		}
		else if (GetStamina() >= 40 && GetHealthEffectByClass(UTiredEffect::StaticClass()) != nullptr)
		{
			RemoveHealthEffect(GetHealthEffectByClass(UTiredEffect::StaticClass()));
		}
		break;
	default:
		break;
	}
	
	if (Cast<APlayerCharacter>(GetOwner()))
	{
		UpdateHealthWidget(HealthType);
		UpdateHealthWidget(EHealthType::Health);
	}
}

void UHealthComponent::AddHealthEffect(TSubclassOf<UHealthEffect> HealthEffectClass)
{
	UHealthEffect* _CreatedEffect = NewObject<UHealthEffect>(GetOwner(), HealthEffectClass);
	HealthEffects.Add(_CreatedEffect);
	GetHealthWidget()->AddHealthEffect(_CreatedEffect);
	_CreatedEffect->StartEffect(this);
}

void UHealthComponent::RemoveHealthEffect(UHealthEffect* HealthEffectReference)
{
	if (GetHealthEffectByClass(HealthEffectReference->GetClass()) != nullptr)
	{
		HealthEffectReference->EffectStoped();
		HealthEffects.Remove(HealthEffectReference);
		GetHealthWidget()->RemoveHealthEffect(HealthEffectReference);
	}
}

void UHealthComponent::ZeroHealth()
{
	DeathDelegate.Broadcast(this);
	Death();
}

void UHealthComponent::Death()
{
}

UHealthEffect* UHealthComponent::GetHealthEffectByClass(TSubclassOf<UHealthEffect> HealthEffectClass)
{
	for (UHealthEffect* _CurrentEffect : GetHealthEffects())
	{
		if (_CurrentEffect->GetClass() == HealthEffectClass)
		{
			return _CurrentEffect;
		}
	}
	return nullptr;
}