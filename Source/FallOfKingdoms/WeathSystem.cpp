// Fill out your copyright notice in the Description page of Project Settings.


#include "WeathSystem.h"
#include "Components/SceneComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/ExponentialHeightFogComponent.h"
#include "Components/SkyAtmosphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AWeathSystem::AWeathSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	WeathRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeathRoot"));
	SunRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SunRoot"));
	SunLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("SunLight"));
	MoonRoot = CreateDefaultSubobject<USceneComponent>(TEXT("MoonRoot"));
	MoonLight = CreateDefaultSubobject<UDirectionalLightComponent>(TEXT("MoonLight"));
	SkyLight = CreateDefaultSubobject<USkyLightComponent>(TEXT("SkyLight"));
	SkyAtmosphere = CreateDefaultSubobject<USkyAtmosphereComponent>(TEXT("SkyAtmosphere"));
	HeightFog = CreateDefaultSubobject<UExponentialHeightFogComponent>(TEXT("HeightFog"));

	RootComponent = WeathRoot;
	GetSunRoot()->SetupAttachment(RootComponent);
	GetSunLight()->SetupAttachment(SunRoot);
	GetMoonRoot()->SetupAttachment(RootComponent);
	GetMoonLight()->SetupAttachment(MoonRoot);
	GetSkyLight()->SetupAttachment(RootComponent);
	GetSkyAtmosphere()->SetupAttachment(RootComponent);
	GetHeightFog()->SetupAttachment(SkyAtmosphere);

	GetMoonLight()->SetUseTemperature(true);
	GetMoonLight()->SetTemperature(8500);

	UpdateDelay = 60;
	UpdateSpeed = 1;
	SunLightIntensity = 7;
	MoonLightIntensity = 0.05;
}

void AWeathSystem::OnConstruction(const FTransform& Transform)
{
	CurrentTime = GetCurrentTime() > 2400 || GetCurrentTime() < 0 ? 0 : CurrentTime;
	
	GetSunLight()->SetRelativeLocation(FVector(0, 0, -100));
	GetSunLight()->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetSunLight()->GetRelativeLocation(), FVector(0, 0, 0)));

	GetMoonLight()->SetRelativeLocation(FVector(100, 0, 100));
	GetMoonLight()->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetMoonLight()->GetRelativeLocation(), FVector(0, 0, 0)));

	UpdateVisuals();
	//GetMoonLight()->SetRelativeRotation(UKismetMathLibrary::FindLookAtRotation(GetMoonLight()->GetRelativeLocation(), FVector(0, 0, 0)));
}

// Called when the game starts or when spawned
void AWeathSystem::BeginPlay()
{
	Super::BeginPlay();
	

	GetWorld()->GetTimerManager().SetTimer(UpdateTimeHandle, this, &AWeathSystem::UpdateWeath, GetUpdateDelay(), true);
}

void AWeathSystem::UpdateWeath()
{
	CurrentTime += GetUpdateDelay() * GetUpdateSpeed();
	CurrentTime = GetCurrentTime() > 2400 || GetCurrentTime() < 0 ? 0 : CurrentTime;

	UpdateVisuals();
}

void AWeathSystem::UpdateVisuals()
{
	float _DayLength = CurrentTime / 2400;

	float _SunLightTemperature = _DayLength > 0.4 ? 3000 : 4500;

	float _SunLightIntensity = GetSunLightIntensityCurve()->GetFloatValue(CurrentTime);
	float _MoonLightIntensity = GetMoonLightIntensityCurve()->GetFloatValue(CurrentTime);

	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), _DayLength, _SunLightIntensity, _MoonLightIntensity);

	
	FRotator _SunRootRotation; _SunRootRotation.Roll = -360 * _DayLength;
	GetSunRoot()->SetRelativeRotation(_SunRootRotation);
	GetSunLight()->SetIntensity(_SunLightIntensity * GetSunLightIntensity());
	GetSunLight()->SetTemperature(_SunLightTemperature + (_SunLightIntensity * 1000));

	FRotator _MoonRootRotation; _MoonRootRotation.Yaw = -360 * _DayLength;
	GetMoonRoot()->SetRelativeRotation(_MoonRootRotation);
	GetMoonLight()->SetIntensity(_MoonLightIntensity * GetMoonLightIntensity());

	//GetSkyLight()->SetIntensity((GetSunLightIntensityCurve()->GetFloatValue(GetCurrentTime()) * 3) + 0.15f);
	GetSkyLight()->RecaptureSky();

	GetHeightFog()->SetFogMaxOpacity(1 - (_MoonLightIntensity * 0.95f));
}