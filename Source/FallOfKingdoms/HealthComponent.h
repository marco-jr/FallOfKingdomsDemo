// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

class UHealthEffect;
class UHealthWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeathDelegate, UHealthComponent*, HealthReference);

UENUM()
enum class EHealthType : uint8
{
	HealthQuality,
	Health,
	Calories,
	Hidratation,
	Stamina
};
ENUM_RANGE_BY_COUNT(EHealthType, 5);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Logic")
		UHealthWidget* HealthWidget;
		UHealthWidget* GetHealthWidget() const { return HealthWidget; }

	UPROPERTY()
		FDeathDelegate DeathDelegate;
		
	// FUNCTIONS
	UFUNCTION()
		UHealthEffect* GetHealthEffectByClass(TSubclassOf<UHealthEffect> HealthEffectClass);
	
		float GetCalories() const { return Calories; }

		bool GetConsumeHealth() const { return bConsumeHealth; }

		float GetHealthQuality() const { return HealthQuality; }

		int GetHealth() const { return Health; }

		float GetStamina() const { return Stamina; }

		float GetHidratation() const { return Hidratation; }


	// METHODS
	UFUNCTION()
		void ChangeHealthByValue(EHealthType HealthType, float Value);

	UFUNCTION()
		void AddHealthEffect(TSubclassOf<UHealthEffect> HealthEffectClass);

	UFUNCTION()
		void RemoveHealthEffect(UHealthEffect* HealthEffectReference);

	UFUNCTION()
		void ZeroHealth();

	UFUNCTION()
		virtual void Death();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	// PROPERTIES
 	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UHealthWidget> HealthWidgetClass;
		TSubclassOf<UHealthWidget> GetHealthWidgetClass() const { return HealthWidgetClass; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bConsumeHealth;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float HealthConsumptionDelay;
		float GetHealthConsumptionDelay() const { return HealthConsumptionDelay; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float HealthQuality;

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		int Health;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float Stamina;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float Calories;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float Hidratation;

	UPROPERTY(VisibleAnywhere, Category = "Logic")
		TArray<UHealthEffect*> HealthEffects;
		TArray<UHealthEffect*> GetHealthEffects() const { return HealthEffects; }

	FTimerHandle ConsumeHealthTimer;

	// FUNCTIONS
	UFUNCTION()
		bool VerifyDehydration();

	UFUNCTION()
		bool VerifyHungry();

	// METHODS
	UFUNCTION()
		void ConsumeHealth();

	UFUNCTION()
		void UpdateHealthWidget(EHealthType HealthType);
};