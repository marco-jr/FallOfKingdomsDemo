// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterIdentity.h"
#include "CharacterController.h"

UCharacterIdentity::UCharacterIdentity()
{
	CharacterName = FText::FromString("Actor Name");
	IdentifierType = EIdentifierType::Default;
	InitSphereRadius(1000);
	SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SetCollisionObjectType(ECC_WorldDynamic);
	SetCollisionResponseToAllChannels(ECR_Ignore);
	SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}