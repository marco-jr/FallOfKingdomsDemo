// Fill out your copyright notice in the Description pag	e of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionComponent.generated.h"

class ACharacter;
class UHealthComponent;
class UActionWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionInterruptedSignature, UActionComponent*, ActionReference);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FActionCompletedSignature, UActionComponent*, ActionReference);

UENUM(BlueprintType)
enum class ECurrentAction : uint8
{
	Ready,
	Interact,
	Attack,
	Busy,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionComponent();

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UActionWidget* ActionWidget;
		UActionWidget* GetActionWidget() const { return ActionWidget; }

	// VARIABLES AND SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bIsHumanoid;
		bool GetIsHumanoid() const { return bIsHumanoid; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bPlayFootstep;
		bool GetPlayFootstep() const { return bPlayFootstep; }

	UPROPERTY(BlueprintAssignable, Category = "Events")
		FActionInterruptedSignature ActionInterruptedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
		FActionCompletedSignature ActionCompletedDelegate;

	// FUNCTIONS
	UFUNCTION()
		void Ready();

	UFUNCTION()
		void Busy();

	UFUNCTION()
		ECurrentAction GetCurrentAction() const { return CurrentAction; }

	UFUNCTION()
		bool GetCanDoActions(bool bShowBusyMessage = false);

	// METHODS
	UFUNCTION()
		void Attack();

	UFUNCTION()
		void Sprint();

	UFUNCTION()
		void StopSprint();

	UFUNCTION()
		void DoAction(ECurrentAction NewAction, int ActionID, bool bShowActionProgress = true, float CustomDuration = 0);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// VARIABLES
	float CurrentActionStartTime;
	float CurrentActionDuration;
	float ExhaustTime;

	FTimerHandle SprintConsumptionTimer;
	FTimerHandle ActionCompleteTimer;

	UPROPERTY(VisibleAnywhere, Transient)
		UHealthComponent* OwnerHealth;
		UHealthComponent* GetOwnerHealth() const { return OwnerHealth; }

	UPROPERTY(VisibleAnywhere, Transient)
		ECurrentAction CurrentAction;

	UPROPERTY(VisibleAnywhere, Transient)
		int CurrentActionID;
		int GetCurrentActionID() const { return CurrentActionID; }
	
	UPROPERTY(VisibleAnywhere, Transient)
		bool bSprinting;
		bool GetIsSprinting() const { return bSprinting; }

	UPROPERTY(VisibleAnywhere, Transient)
		bool bCannotMove;
		bool GetCannotMove() const { return bCannotMove; }

	UPROPERTY(VisibleAnywhere, Transient)
		ACharacter* OwnerCharacter;
		ACharacter* GetOwnerCharacter() const { return OwnerCharacter; }

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		FText BusyMessage;
		FText GetBusyMessage() const { return BusyMessage; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		TMap<ECurrentAction, float> DefaultActionsDuration;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<ECurrentAction> DefaultActionsStopMovement;

	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UActionWidget> ActionWidgetClass;

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
		void ActionCompleted();

	UFUNCTION()
		void ActionInterrupted();

	UFUNCTION()
		void UpdateMovementSpeed();

	UFUNCTION()
		void SetCannotMove();

	UFUNCTION()
		void SetCanMove();

	UFUNCTION()
		void SprintConsumption();
};