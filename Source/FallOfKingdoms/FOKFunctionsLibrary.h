// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "FOKFunctionsLibrary.generated.h"

class UHandComponent;
class AActor;

UCLASS()
class FALLOFKINGDOMS_API UFOKFunctionsLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION()
		static UHandComponent* GetFreeHand(AActor* ActorReference);

	UFUNCTION()
		static UHandComponent* GetMainHand(AActor* ActorReference);
};
