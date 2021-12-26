// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/UserDefinedEnum.h"
#include "Kismet/GameplayStatics.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Team5_GP3/AI/Customer Variation/AIChildCustomer.h"
#include "Team5_GP3/AI/Customer Variation/AIFighterCustomer.h"
#include "Team5_GP3/AI/Others/StoreItem.h"


bool UGameManager::Tick(float DeltaSeconds)
{
	if(UGameplayStatics::GetCurrentLevelName(this,true) != "DonutLevel") return true;
		
	if (UGameplayStatics::IsGamePaused(GetWorld())) 
	{ 
		return true;
	}
	timeAccumulated += DeltaSeconds;
	SpawnCustomer();
	PlayerLocation = UGameplayStatics::GetPlayerPawn(this, 0)->GetActorLocation();
/*
	if(Today == EDays::D_Monday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's Monday"));
	}
	if(Today == EDays::D_Tuesday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Tuesday"));
	}
	if(Today == EDays::D_Wednesday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Wednesday"));
	}
	if(Today == EDays::D_Thursday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Thursday"));
	}
	if(Today == EDays::D_Friday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Friday"));
	}
	if(Today == EDays::D_Saturday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Saturday"));
	}
	if(Today == EDays::D_Sunday)
	{
		UE_LOG(LogTemp,Warning,TEXT("It's D_Sunday"));
	}
*/
	return true;
}



void UGameManager::Init()
{
		TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UGameManager::Tick));
		Super::Init();
		ItemsInStoreFromStart = StoreItems.Num()+1;
}

void UGameManager::Shutdown()
{
	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Super::Shutdown();
}





void UGameManager::SpawnCustomer()
{
	if(UGameplayStatics::GetCurrentLevelName(this,true) != "DonutLevel") return;

	AAICustomerAbstract* AICustomerAbs = nullptr;
	const int16 RandSpawnLocation = FMath::RandRange(0, SpawnLocations.Num() - 1);
	const FActorSpawnParameters SpawnInfo;

	if (isSpawningCond && timeAccumulated > cooldown && StoreItems.Num() > 0)
	{
		timeAccumulated = 0;
		AICustomerAbs = GetWorld()->SpawnActor<AAICustomerAbstract>(GetCustomerBasedOnWeightedProbability(), SpawnLocations[RandSpawnLocation], {0, 0, 0}, SpawnInfo);
	}

	if(AICustomerAbs!=nullptr)
	{
		const int16 ChildChance =  FMath::RandRange(0, 100);
		FVector tempLocation = SpawnLocations[RandSpawnLocation];
		tempLocation.X = tempLocation.X - 100;
		if(ChildSpawn.DayBaseProbability.Contains(Today))
		{
			if(ChildChance < ChildSpawn.DayBaseProbability[Today] && !AICustomerAbs->IsA(AAIFighterCustomer::StaticClass()))
			{
				AAIChildCustomer* Child = GetWorld()->SpawnActor<AAIChildCustomer>(ChildSpawn.AICustomerToSpawn, tempLocation, {0, 0, 0}, SpawnInfo);			
				Child->SetParent(AICustomerAbs);
				AICustomerAbs->CustomerComponent->HasChild = true; // I dont know the reason of this boolean but gonna fill it up . 
			}
		}
	}

	
}


void UGameManager::ShuffleStoreItems()
{
	const int32 NumShuffles = StoreItems.Num() - 1;
	for (int32 i = 0; i < NumShuffles; ++i)
	{
		const FRandomStream Rand;
		const int32 SwapIdx = Rand.RandRange(i, NumShuffles);
		StoreItems.Swap(i, SwapIdx);
	}
}


TSubclassOf<AAICustomerAbstract> UGameManager::GetCustomerBasedOnWeightedProbability()
{
	int totalProb = 0;

	for(int counter = 0; counter < SpawnData.Num(); counter ++ )
	{
		if(!SpawnData[counter].DayBaseProbability.Contains(Today)){totalProb +=0; continue;}
		totalProb += SpawnData[counter].DayBaseProbability[Today];
	}

	int randomIndex = FMath::RandRange(0, totalProb);

	for(int counter = 0; counter < SpawnData.Num(); counter ++ )
	{
		const FSpawnCustomerStruct CustomerStruct = SpawnData[counter];

		if(SpawnData[counter].DayBaseProbability.Contains(Today))
		{
			if(randomIndex < CustomerStruct.DayBaseProbability[Today])
			{
				return CustomerStruct.AICustomerToSpawn;
			}
			randomIndex -= CustomerStruct.DayBaseProbability[Today];
		}
	}

	// Just to not return a nullPtr ( Still can be one ) But Going to return in For Loop above
	return SpawnData[0].AICustomerToSpawn;
}

TArray<AAICustomerAbstract*> UGameManager::GetAllCustomers()
{
	return Customers;
}




void UGameManager::BeginSpawnCustomers()
{
	ShuffleStoreItems();
	if (GetSpawnLocationCount() > 0) isSpawningCond = true;
}


float UGameManager::GetPlayerDirection(AActor* AICustomer) const
{
	const float playerPawnX = GetPlayerLocation().X;
	const float playerPawnY = GetPlayerLocation().Y;
	const float ownerPawnX = AICustomer->GetActorLocation().X;
	const float ownerPawnY = AICustomer->GetActorLocation().Y;
		
	return atan((playerPawnY - ownerPawnY) / (playerPawnX - ownerPawnX));
	
	return 0;
}

AStoreItem* UGameManager::PopAStoreItem()
{
	int32 randArrayIndex = UKismetMathLibrary::RandomInteger(StoreItems.Num());
	
	if(StoreItems.Num() > 0)
	{
		return StoreItems[randArrayIndex];
	}else
	{
		return nullptr;
	}
}

void UGameManager::RemoveAllCustomers()
{
	for (AAICustomerAbstract* CustomerAbstract : Customers)
	{
		CustomerAbstract->DeleteAllItems();
		Customers.Remove(CustomerAbstract);
		CustomerAbstract->Destroy();
	}
	Customers.Empty();
}

AStoreItem* UGameManager::CloneStoreItem(AStoreItem* ItemToClone)
{
	return ItemToClone = NewObject<AStoreItem>();
}

