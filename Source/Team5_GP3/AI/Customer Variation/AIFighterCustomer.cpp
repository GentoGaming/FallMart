// Fill out your copyright notice in the Description page of Project Settings.


#include "AIFighterCustomer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/AI/FighterComponent.h"


AAIFighterCustomer::AAIFighterCustomer()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAIFighterCustomer::UpdateBlackboard()
{
}

void AAIFighterCustomer::CheckDoneShopping()
{
	if (GetAllPickedItems().Num() <= itemsToShop)
	{
		BlackboardComponent->SetValueAsBool(TEXT("FinnishedShopping"), GetAllPickedItems().Num() <= itemsToShop);
		isFinishedShopping = true;
	}
}

void AAIFighterCustomer::IsInFightingArea()
{
	// if(FVector)
}

void AAIFighterCustomer::CheckStartShopping()
{
	if (!inStore)
	{
		if (FVector::PointsAreNear(GetActorLocation(), StoreEntryPoint, DistanceToStoreLocation))
		{
			inStore = true;
			BlackboardComponent->SetValueAsBool(TEXT("StartShopping"), inStore);
		}
	}
}

void AAIFighterCustomer::CheckIsInFighterArea()
{
	if (!inFightingArea && BlackboardComponent->GetValueAsBool(TEXT("FinnishedShopping")) == true)
	{
		if (FVector::PointsAreNear(GetActorLocation(), FightingArea, DistanceToStoreLocation) )
		{
			inFightingArea = true;
			BlackboardComponent->SetValueAsBool(TEXT("IsInFighterArea"), inFightingArea);
		}
	}
}

void AAIFighterCustomer::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// CheckDoneShopping();
	CheckStartShopping();
	CheckIsInFighterArea();

	if (inFightingArea)
	{
	}
}

void AAIFighterCustomer::BeginPlay()
{
	Super::BeginPlay();
	if (BlackboardComponent == nullptr) return;
	FightingArea = BlackboardComponent->GetValueAsVector(TEXT("FightingAreaLocation"));
	FighterComponent = Cast<UFighterComponent>(
		GetComponentByClass(TSubclassOf<UActorComponent>(UFighterComponent::StaticClass())));
	BlackboardComponent->SetValueAsInt(TEXT("ItemsToShopAmount"), itemsToShop);

}
