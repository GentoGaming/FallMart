// Fill out your copyright notice in the Description page of Project Settings.


#include "FighterComponent.h"

// Sets default values for this component's properties
UFighterComponent::UFighterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UFighterComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UFighterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UFighterComponent::SetFightingPartner(UFighterComponent* OtherFighterCustomer)
{
	FightingPartner = OtherFighterCustomer;
}

UFighterComponent* UFighterComponent::GetFightingPartner()
{
	return FightingPartner;
}

