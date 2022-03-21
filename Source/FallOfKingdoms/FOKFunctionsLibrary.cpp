// Fill out your copyright notice in the Description page of Project Settings.


#include "FOKFunctionsLibrary.h"
#include "GameFramework/Actor.h"
#include "HandComponent.h"

UHandComponent* UFOKFunctionsLibrary::GetFreeHand(AActor* ActorReference)
{
	TArray<USceneComponent*> _ReturnComponents;
	TArray<UHandComponent*> _HandsReference;
	ActorReference->GetRootComponent()->GetChildrenComponents(true, _ReturnComponents);

	for (auto& _CurrentComponent : _ReturnComponents)
	{
		if (UHandComponent* _CurrentHand = Cast<UHandComponent>(_CurrentComponent))
		{
			_HandsReference.Add(_CurrentHand);
		}
	}

	UHandComponent* _LastFreeHand = nullptr;

	for (UHandComponent* _CurrentHand : _HandsReference)
	{
		if (_CurrentHand->GetItemObject() == nullptr)
		{
			_LastFreeHand = _CurrentHand;
			if (_CurrentHand->GetIsMainHand())
			{
				return _CurrentHand;
			}
		}
	}

	if (_LastFreeHand != nullptr)
	{
		return _LastFreeHand;
	}

	return nullptr;
}

UHandComponent* UFOKFunctionsLibrary::GetMainHand(AActor* ActorReference)
{
	if (ActorReference != nullptr)
	{
		TArray<USceneComponent*> _ChildrenComponents;
		ActorReference->GetRootComponent()->GetChildrenComponents(true, _ChildrenComponents);
		TArray<UHandComponent*> _HandComponents;

		for (auto& _CurrentIndex : _ChildrenComponents)
		{
			if (UHandComponent* _CurrentHand = Cast<UHandComponent>(_CurrentIndex))
			{
				_HandComponents.Add(_CurrentHand);
			}
		}

		for (UHandComponent* _CurrentHand : _HandComponents)
		{
			if (_CurrentHand->GetIsMainHand())
			{
				return _CurrentHand;
			}
		}
	}
	return nullptr;
}
