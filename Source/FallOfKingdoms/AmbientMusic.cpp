// Fill out your copyright notice in the Description page of Project Settings.


#include "AmbientMusic.h"
#include "Components/SphereComponent.h"
#include "Components/SceneComponent.h"
#include "PlayerCharacter.h"
#include "Components/AudioComponent.h"

AAmbientMusic::AAmbientMusic()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	CommunicationSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CommunicationSphere"));

	RootComponent = SceneComponent;
	GetCommunicationSphere()->SetupAttachment(RootComponent);
	
	SphereRadius = 2000;
	MinimalVolumeMultiplier = 0.35;
	bDynamicMusicVolume = true;

	GetCommunicationSphere()->OnComponentBeginOverlap.AddDynamic(this, &AAmbientMusic::CommunicationBeginOverlap);
	GetCommunicationSphere()->OnComponentEndOverlap.AddDynamic(this, &AAmbientMusic::CommunicationEndOverlap);
}

USoundBase* AAmbientMusic::GetRandomMusicSound()
{
	USoundBase* _RandomSound = GetMusicSounds()[FMath::RandRange(0, GetMusicSounds().Num() - 1)];
	return _RandomSound != nullptr ? _RandomSound : nullptr;
	return nullptr;
}

void AAmbientMusic::BeginPlay()
{

}

void AAmbientMusic::OnConstruction(const FTransform& Transform)
{
	GetCommunicationSphere()->InitSphereRadius(GetSphereRadius());
}

void AAmbientMusic::CommunicationBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(OtherActor))
	{
		StartMusic(_CharacterReference);
	}
}

void AAmbientMusic::CommunicationEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (APlayerCharacter* _CharacterReference = Cast<APlayerCharacter>(OtherActor))
	{
		if (_CharacterReference == GetCharacterReference())
		{
			StopMusic();
		}
	}
}

void AAmbientMusic::StartMusic(APlayerCharacter* NewCharacter)
{
	CharacterReference = NewCharacter;

	if (GetDynamicMusicVolume())
	{
		GetWorld()->GetTimerManager().SetTimer(MusicUpdateHandle, this, &AAmbientMusic::UpdateMusicVolume, 0.1, true);
	}
	
	float _StartSound = GetDynamicMusicVolume() ? (1 - FVector::Dist2D(GetCharacterReference()->GetActorLocation(), GetActorLocation()) / GetSphereRadius()) + GetMinimalVolumeMultiplier() : 1;
	GetCharacterReference()->PlayMusic(this, _StartSound);
}

void AAmbientMusic::StopMusic()
{
	if (GetCharacterReference() != nullptr)
	{
		GetWorld()->GetTimerManager().ClearTimer(MusicUpdateHandle);
		GetCharacterReference()->PlayMusic();
		CharacterReference = nullptr;
	}
}

void AAmbientMusic::UpdateMusicVolume()
{
	if (GetCharacterReference() != nullptr)
	{
		float _VolumeMultiplier = (1 - FVector::Dist2D(GetCharacterReference()->GetActorLocation(), GetActorLocation()) / GetSphereRadius()) + GetMinimalVolumeMultiplier();
		_VolumeMultiplier = _VolumeMultiplier > 1 ? 1 : _VolumeMultiplier;
		GetCharacterReference()->GetMusicAudioComponent()->SetVolumeMultiplier(_VolumeMultiplier * GetCharacterReference()->MusicVolume);
		GetCharacterReference()->GetDefaultMusicAudioComponent()->SetVolumeMultiplier((1 - _VolumeMultiplier) * GetCharacterReference()->MusicVolume);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(MusicUpdateHandle);
	}
}
