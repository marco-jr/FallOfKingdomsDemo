// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FallOfKingdomsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class FALLOFKINGDOMS_API AFallOfKingdomsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFallOfKingdomsGameModeBase();

	UPROPERTY(EditAnywhere, Category = "Talk Settings")
		float TextAnimSpeed;
		float GetTextAnimSpeed() const { return TextAnimSpeed; }
};
