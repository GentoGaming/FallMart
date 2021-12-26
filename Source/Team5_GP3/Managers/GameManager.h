// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Team5_GP3/AI/Others/SpawnCustomerStruct.h"

#include "GameManager.generated.h"


class UUserDefinedEnum;
class AStoreItem;
class AAICustomerAbstract;


UCLASS()
class TEAM5_GP3_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
private:
	virtual void Init() override;
	virtual void Shutdown() override;
	EDays Today = EDays::D_Monday;
	bool Tick(float DeltaSeconds);
	FDelegateHandle TickDelegateHandle;

	UPROPERTY()
	TArray<FVector> SpawnLocations;
	UPROPERTY()
	TArray<FVector> ShelvesLocation;
	UPROPERTY()
	TArray<AAICustomerAbstract*> Customers;

	void SpawnCustomer();
	void ShuffleStoreItems();
	TSubclassOf<AAICustomerAbstract> GetCustomerBasedOnWeightedProbability();
	float cooldown = 2.0f, timeAccumulated = 0.f;
	bool isSpawningCond = false;
	FVector StoreEnterLocationVector, StoreExitLocationVector, StoreCashierLocationVector, PlayerLocation,
	        TPFightingAreaLocation;

public:
	UPROPERTY()
	TArray<AStoreItem*> StoreItems;
	UPROPERTY(EditAnywhere)
	int maxItemsToBePicked = 3;

	UFUNCTION(BlueprintPure)
	int GetMaxItemsToBePicked() const { return maxItemsToBePicked; };

	UFUNCTION(BlueprintPure)
	TArray<AAICustomerAbstract*> GetAllCustomers();

	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	TArray<FSpawnCustomerStruct> SpawnData;
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	FSpawnCustomerStruct ChildSpawn;
	UPROPERTY(EditDefaultsOnly, Category="Spawning")
	int childCrazinessProbability = 50; // chance that child gonna jump on another customer

	UFUNCTION(BlueprintCallable)
	TArray<FSpawnCustomerStruct> GetSpawnData() { return SpawnData; };
	UFUNCTION(BlueprintCallable)
	FSpawnCustomerStruct GetChildData() { return ChildSpawn; };

	int GetSpawnLocationCount() const { return SpawnLocations.Num(); }
	int GetShelvesCount() const { return ShelvesLocation.Num(); }
	int GetCustomersCount() const { return Customers.Num(); }

	FVector GetPlayerLocation() const { return PlayerLocation; };

	// Adding Removing Customers
	UFUNCTION(BlueprintCallable)
	void AddCustomers(AAICustomerAbstract* Customer) { Customers.Add(Customer); };
	UFUNCTION(BlueprintCallable)
	void RemoveCustomers(AAICustomerAbstract* Customer) { Customers.Remove(Customer); };
	int ItemsInStoreFromStart;
	float GetPlayerDirection(AActor* AICustomer) const;
	AStoreItem* PopAStoreItem();
	// Adding Removing Store Items
	UFUNCTION(BlueprintCallable)
	void AddStoreItem(AStoreItem* OneItem) { StoreItems.Add(OneItem); };
	UFUNCTION(BlueprintCallable)
	void RemoveStoreItem(AStoreItem* OneItem) { StoreItems.Remove(OneItem); };
	UFUNCTION(BlueprintCallable)
	void RemoveAllCustomers();
	AStoreItem* CloneStoreItem(AStoreItem* ItemToClone);
	// Adding Waypoints for Entry , Exit , Special places Waypoints
	UFUNCTION(BlueprintCallable)
	void AddSpawnLocations(const FVector ALocation) { SpawnLocations.Add(ALocation); };

	UFUNCTION(BlueprintCallable)
	void AddShelvesLocations(const FVector ALocation) { ShelvesLocation.Add(ALocation); };

	UFUNCTION(BlueprintCallable)
	void AddStoreEnterLocation(const FVector ALocation) { StoreEnterLocationVector = ALocation; }

	UFUNCTION(BlueprintCallable)
	void AddStoreExitLocation(const FVector ALocation) { StoreExitLocationVector = ALocation; }

	UFUNCTION(BlueprintCallable)
	void AddStoreCashierLocation(const FVector ALocation) { StoreCashierLocationVector = ALocation; }

	UFUNCTION(BlueprintCallable)
	void SetTPFightingAreaLocation(const FVector ALocation) { TPFightingAreaLocation = ALocation; }

	// Spawn CoolDown Change 
	UFUNCTION(BlueprintCallable)
	void ChangeCooldown(const float NewCoolDown)
	{
		cooldown = NewCoolDown;
	};

	// Getting Waypoints for Entry , Exit , Special places Waypoints
	UFUNCTION(BlueprintCallable)
	FVector GetStoreEntryLocation() const { return StoreEnterLocationVector; };

	UFUNCTION(BlueprintCallable)
	FVector GetStoreExitLocation() const { return StoreExitLocationVector; };

	UFUNCTION(BlueprintCallable)
	FVector GetCashierLocation() const { return StoreCashierLocationVector; };

	UFUNCTION(BlueprintCallable)
	FVector GetFightingAreaLocation() const { return TPFightingAreaLocation; }

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations() const { return SpawnLocations; }

	UFUNCTION(BlueprintCallable)
	void StopSpawnCustomers() { isSpawningCond = false; }

	UFUNCTION(BlueprintCallable)
	void BeginSpawnCustomers();

	UFUNCTION(BlueprintCallable)
	void SetDay(EDays D_Today){Today = D_Today; };
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnThreatChange(float threatValue);
};
