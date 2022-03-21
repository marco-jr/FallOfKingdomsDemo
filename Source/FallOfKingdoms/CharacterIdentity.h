// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "CharacterIdentity.generated.h"

UENUM()
enum class EIdentifierType : uint8
{
	Default,
	Wildlife,
	Hostile
};

UCLASS()
class FALLOFKINGDOMS_API UCharacterIdentity : public USphereComponent
{
	GENERATED_BODY()

public:
	UCharacterIdentity();

	// PROPERTIES
	UPROPERTY(EditAnywhere, Category = "Settings")
		FText CharacterName;
		FText GetCharacterName() const { return CharacterName; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		EIdentifierType IdentifierType;
		EIdentifierType GetIdentifierType() const { return IdentifierType; }
};
