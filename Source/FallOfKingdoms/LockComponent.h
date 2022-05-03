// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULockComponent();

	UFUNCTION()
		bool GetIsLocked() const { return bLocked; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;	

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		int LockID;
		int GetLockID() const { return LockID; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		bool bLocked;
};
