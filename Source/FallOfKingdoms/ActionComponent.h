// Fill out your copyright notice in the Description pag	e of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class ACharacter;

UENUM()
enum class ECurrentAction : uint8
{
	Ready,
	Interact,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	// METHODS
	UFUNCTION()
		void Sprint();

	UFUNCTION()
		void StopSprint();

	UFUNCTION()
		void DoAction()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// VARIABLES
	ACharacter* OwnerCharacter;
	ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	FTimerHandle SprintConsumptionTimer;

	ECurrentAction CurrentAction;

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		float WalkSpeed;
		float GetWalkSpeed() const { return WalkSpeed; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float SprintSpeed;
		float GetSprintSpeed() const { return SprintSpeed; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float Acceleration;
		float GetAcceleration() const { return Acceleration; }

	// METHODS
	UFUNCTION()
		void SetMovementSpeed(bool Sprint);

	UFUNCTION()
		void SprintConsumption();
};