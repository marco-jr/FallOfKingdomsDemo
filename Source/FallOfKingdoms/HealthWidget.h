// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

class UImage;
class UProgressBar;
class UHealthComponent;
class UHorizontalBox;
class UHealthEffectWidget;
class UHealthEffect;
class UTexture2D;
class UVisualEffectWidget;

USTRUCT(BlueprintType)
struct FHealthEffectSetting
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
		UTexture2D* EffectImage;

	UPROPERTY(EditAnywhere)
		FLinearColor ImageColor;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UVisualEffectWidget> VisualEffectWidget;
};

UCLASS()
class FALLOFKINGDOMS_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		FLinearColor NormalStatusColor;
		FLinearColor GetNormalStatusColor() const { return NormalStatusColor; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		FLinearColor ErrorStatusColor;
		FLinearColor GetErrorStatusColor() const { return ErrorStatusColor; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TMap<TSubclassOf<UHealthEffect>, FHealthEffectSetting> EffectsSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
		TSubclassOf<UHealthEffectWidget> HealthEffectWidgetClass;
		TSubclassOf<UHealthEffectWidget> GetHealthEffectWidgetClass() const { return HealthEffectWidgetClass; }

	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* HungryAnimation;
		UWidgetAnimation* GetHungryAnimation() const { return HungryAnimation; }

	UPROPERTY(VisibleAnywhere, Category = "Animation", meta = (BindWidgetAnim), Transient)
		UWidgetAnimation* ThirstyAnimation;
		UWidgetAnimation* GetThirstyAnimation() const { return ThirstyAnimation; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UHorizontalBox* EffectsHorizontalBox;
		UHorizontalBox* GetEffectsHorizontalBox() const { return EffectsHorizontalBox; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* HidratationImage;
		UImage* GetHidratationImage() const { return HidratationImage; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UImage* CaloriesImage;
		UImage* GetCaloriesImage() const { return CaloriesImage; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthBar;
		UProgressBar* GetHealthBar() const { return HealthBar; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HealthQualityBar;
		UProgressBar* GetHealthQualityBar() const { return HealthQualityBar; }

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* StaminaBar;
		UProgressBar* GetStaminaBar() const { return StaminaBar; }

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		TMap<UHealthEffect*, UHealthEffectWidget*> HealthEffects;

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		TMap<UHealthEffect*, UVisualEffectWidget*> VisualEffects;

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		UHealthComponent* HealthComponent;
		UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	UFUNCTION()
		void UpdateHealth(EHealthType HealthType, float Value);

	UFUNCTION()
		void AddHealthEffect(UHealthEffect* HealthEffectReference);

	UFUNCTION()
		void RemoveHealthEffect(UHealthEffect* HealthEffectReference);

	UFUNCTION()
		void SetHungry(bool bIsHungry);

	UFUNCTION()
		void SetThirsty(bool bIsThirsty);
};
