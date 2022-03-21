// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "HandComponent.generated.h"

class AItemObject;
class UHandWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UHandComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHandComponent();

	// COMPONENTS
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Components")
		UHandWidget* HandWidget;
		UHandWidget* GetHandWidget() const { return HandWidget; }

	// FUNCTIONS
	AItemObject* GetItemObject() const { return ItemObject; }
	float GetArmLength() const { return ArmLength; }
	bool GetIsMainHand() const { return bMainHand; }

	// METHODS
	UFUNCTION()
		void AddItemObject(AItemObject* ItemObjectReference);

	UFUNCTION()
		void RemoveItemObject(bool bSimulatePhysics);

	UFUNCTION()
		void UseItemObject();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		TSubclassOf<UHandWidget> HandWidgetClass;

	UPROPERTY(VisibleAnywhere, Transient)
		AItemObject* ItemObject;

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bMainHand;

	UPROPERTY(EditAnywhere, Category = "Settings")
		float ArmLength;
};
