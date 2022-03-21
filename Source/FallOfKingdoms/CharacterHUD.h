// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterHUD.generated.h"

class UActionMessageWidget;
class UImage;
class UMaterialInterface;

UCLASS()
class FALLOFKINGDOMS_API UCharacterHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UActionMessageWidget* ActionMessage;
		UActionMessageWidget* GetActionMessage() const { return ActionMessage; }
};