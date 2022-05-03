// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthEffect.generated.h"

class UHealthEffectWidget;
class UTexture2D;
class UHealthComponent;
class UVisualEffectWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UHealthEffect : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthEffect();

	//PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		FText EffectDescription;
		FText GetEffectDescription() const { return EffectDescription; }

	UPROPERTY(EditAnywhere, Category = "Logic")
		UUserWidget* EffectWidget;
		UUserWidget* GetEffectWidget() const { return EffectWidget; }

	UPROPERTY(EditAnywhere, Category = "Logic")
		TSubclassOf<UVisualEffectWidget> VisualEffectWidgetClass;
		TSubclassOf<UVisualEffectWidget> GetVisualEffectWidgetClass() const { return VisualEffectWidgetClass; }

	// METHODS
	UFUNCTION()
		void StartEffect(UHealthComponent* HealthComponentReference);

	UFUNCTION()
		void StopEffect();

	UFUNCTION()
		virtual void EffectStarted();

	UFUNCTION()
		virtual void EffectStoped();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		float EffectDuration;
		float GetEffectDuration() const { return EffectDuration; }

	UPROPERTY(EditAnywhere, Category = "Logic")
		UHealthComponent* HealthComponent;
		UHealthComponent* GetHealthComponent() const { return HealthComponent; }

	FTimerHandle EffectTimer;
};
