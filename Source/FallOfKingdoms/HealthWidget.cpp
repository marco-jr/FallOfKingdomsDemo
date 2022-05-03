// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "HealthComponent.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/Texture2D.h"
#include "HealthEffectWidget.h"
#include "HealthEffect.h"
#include "VisualEffectWidget.h"

void UHealthWidget::UpdateHealth(EHealthType HealthType, float Value)
{
	switch (HealthType)
	{
		case EHealthType::Health:
			GetHealthBar()->SetPercent(Value);
			break;
		case EHealthType::HealthQuality:
			GetHealthQualityBar()->SetPercent(Value);
			break;
		case EHealthType::Stamina:
			GetStaminaBar()->SetPercent(Value);
			break;
		case EHealthType::Calories:
			GetCaloriesImage()->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Value"), Value);
			break;
		case EHealthType::Hidratation:
			GetHidratationImage()->GetDynamicMaterial()->SetScalarParameterValue(TEXT("Value"), Value);
			break;
		default:
			break;
	}
}

void UHealthWidget::AddHealthEffect(UHealthEffect* HealthEffectReference)
{
	if (GetHealthEffectWidgetClass() != nullptr)
	{
		UHealthEffectWidget* _CreatedEffectWidget = CreateWidget<UHealthEffectWidget>(this, GetHealthEffectWidgetClass());
		_CreatedEffectWidget->HealthEffectReference = HealthEffectReference;
		HealthEffects.Add(HealthEffectReference, _CreatedEffectWidget);
		for (TPair<TSubclassOf<UHealthEffect>, FHealthEffectSetting> _CurrentHealthEffect : EffectsSettings)
		{
			if (HealthEffectReference->GetClass() == _CurrentHealthEffect.Key)
			{
				UTexture2D* _EffectTexture = _CurrentHealthEffect.Value.EffectImage;
				FLinearColor _EffectColor = _CurrentHealthEffect.Value.ImageColor;
				_CreatedEffectWidget->SetEffectImageSettings(_EffectTexture, _EffectColor);	
			}
		}
		UHorizontalBoxSlot* _AddSlot = GetEffectsHorizontalBox()->AddChildToHorizontalBox(_CreatedEffectWidget);
		_AddSlot->SetPadding(FMargin(5.0, 0.0));
	}
}

void UHealthWidget::RemoveHealthEffect(UHealthEffect* HealthEffectReference)
{
	if (VisualEffects.Contains(HealthEffectReference))
	{
		GetEffectsHorizontalBox()->RemoveChild(*HealthEffects.Find(HealthEffectReference));
		HealthEffects.Remove(HealthEffectReference);
		if (VisualEffects.Contains(HealthEffectReference))
		{
			UVisualEffectWidget* _VisualEffect = *VisualEffects.Find(HealthEffectReference);
			_VisualEffect->RemoveFromParent();
			_VisualEffect->Destruct();
			VisualEffects.Remove(HealthEffectReference);
		}
	}
}

void UHealthWidget::SetHungry(bool bIsHungry)
{
	bIsHungry ? PlayAnimation(HungryAnimation, 0, 0) : StopAnimation(HungryAnimation);
	GetCaloriesImage()->SetColorAndOpacity(bIsHungry ? ErrorStatusColor : NormalStatusColor);
}

void UHealthWidget::SetThirsty(bool bIsThirsty)
{
	bIsThirsty ? PlayAnimation(ThirstyAnimation, 0, 0) : StopAnimation(ThirstyAnimation);
	GetHidratationImage()->SetColorAndOpacity(bIsThirsty ? ErrorStatusColor : NormalStatusColor);
}
