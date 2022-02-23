// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "InteractableCameraComponent.generated.h"

class USphereComponent;
class UInteractableComponent;

UCLASS()
class FALLOFKINGDOMS_API UInteractableCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

public:
	UInteractableCameraComponent();

protected:
	// CREATE WIDGET ON BEGIN PLAY
	virtual void BeginPlay() override;	
	
};
