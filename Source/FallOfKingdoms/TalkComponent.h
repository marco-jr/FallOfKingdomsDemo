// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "TalkComponent.generated.h"

class ACharacterController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FResponseDelegate, int, TalkIndex, int, ResponseIndex, int, ReturnTalkID);

USTRUCT(BlueprintType)
struct FResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FText ResponseText;

	UPROPERTY(EditAnywhere)
		int ReturnTalkID;
};

USTRUCT(BlueprintType)
struct FTalk
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FText TalkText;

	UPROPERTY(EditAnywhere)
		TArray<FResponse> Responses;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FALLOFKINGDOMS_API UTalkComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTalkComponent();

	// PROPERTIES
	UPROPERTY(VisibleAnywhere, Category = "Logic")
		ACharacterController* CharacterController;
		ACharacterController* GetCharacterController() const { return CharacterController; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<FTalk> TalkSettings;
		TArray<FTalk> GetTalkSettings() const { return TalkSettings; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		TArray<FText> FarewellMessages;
		TArray<FText> GetFarewellMessages() const { return FarewellMessages; }

	UPROPERTY(BlueprintAssignable)
		FResponseDelegate ResponseDelegate;

	FTimerHandle EndTalkHandle;

	// FUNCTIONS
	UFUNCTION()
		FText GetRandomFarewellText();

	UFUNCTION()
		FText GetNPCName();

	// METHODS
	UFUNCTION(BlueprintCallable)
		void StartTalk(ACharacterController* ControllerReference);

	UFUNCTION(BlueprintCallable)
		void EndTalk();

	UFUNCTION()
		void ResponseClicked(int TalkIndex, int ResponseIndex, int ReturnTalkID);

	UFUNCTION()
		FTalk GetTalkByIndex(int Index);

	UFUNCTION()
		bool GetResponseByIndex(int TalkIndex, int ResponseIndex, FResponse &ResponseReturn);

	UFUNCTION()
		void ActorEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void TalkEnded();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
};
