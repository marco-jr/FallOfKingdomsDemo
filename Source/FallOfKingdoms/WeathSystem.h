// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeathSystem.generated.h"

class UDirectionalLightComponent;
class USceneComponent;
class UCurveFloat;
class USkyLightComponent;
class UExponentialHeightFogComponent;
class USkyAtmosphereComponent;

UCLASS()
class FALLOFKINGDOMS_API AWeathSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeathSystem();
	
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* WeathRoot;
		USceneComponent* GetWeathRoot() const { return WeathRoot; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* SunRoot;
		USceneComponent* GetSunRoot() const { return SunRoot; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDirectionalLightComponent* SunLight;
		UDirectionalLightComponent* GetSunLight() const { return SunLight; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* MoonRoot;
		USceneComponent* GetMoonRoot() const { return MoonRoot; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UDirectionalLightComponent* MoonLight;
		UDirectionalLightComponent* GetMoonLight() const { return MoonLight; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkyLightComponent* SkyLight;
		USkyLightComponent* GetSkyLight() const { return SkyLight; }
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USkyAtmosphereComponent* SkyAtmosphere;
		USkyAtmosphereComponent* GetSkyAtmosphere() const { return SkyAtmosphere; }

	UPROPERTY(VisibleAnywhere, Category = "Components")
		UExponentialHeightFogComponent* HeightFog;
		UExponentialHeightFogComponent* GetHeightFog() const { return HeightFog; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;

	// SETTINGS
	UPROPERTY(EditAnywhere, Category = "Settings")
		float UpdateDelay;
		float GetUpdateDelay() const { return UpdateDelay; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float UpdateSpeed;
		float GetUpdateSpeed() const { return UpdateSpeed; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float SunLightIntensity;
		float GetSunLightIntensity() const { return SunLightIntensity; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float MoonLightIntensity;
		float GetMoonLightIntensity() const { return MoonLightIntensity; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UCurveFloat* SunLightIntensityCurve;
		UCurveFloat* GetSunLightIntensityCurve() const { return SunLightIntensityCurve; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		UCurveFloat* MoonLightIntensityCurve;
		UCurveFloat* GetMoonLightIntensityCurve() const { return MoonLightIntensityCurve; }

	UPROPERTY(EditAnywhere, Category = "Settings")
		float CurrentTime;
		float GetCurrentTime() const { return CurrentTime; }

	// METHODS
	UFUNCTION()
		void UpdateWeath();

	UFUNCTION(CallInEditor)
		void UpdateVisuals();

	FTimerHandle UpdateTimeHandle;
};
