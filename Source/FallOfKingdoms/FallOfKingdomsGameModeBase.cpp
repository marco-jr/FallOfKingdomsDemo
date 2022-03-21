// Copyright Epic Games, Inc. All Rights Reserved.


#include "FallOfKingdomsGameModeBase.h"
#include "PlayerCharacter.h"
#include "CharacterController.h"

AFallOfKingdomsGameModeBase::AFallOfKingdomsGameModeBase()
{
	DefaultPawnClass = APlayerCharacter::StaticClass();
	PlayerControllerClass = ACharacterController::StaticClass();
	TextAnimSpeed = 1;
}