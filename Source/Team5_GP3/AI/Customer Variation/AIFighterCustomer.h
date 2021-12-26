// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/AI/FighterComponent.h"

#include "AIFighterCustomer.generated.h"

class UCustomerComponent;
class UFighterComponent;
/**
 * 
 */


UCLASS()
class TEAM5_GP3_API AAIFighterCustomer : public AAICustomerAbstract
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	//int itemsToShop = 1;
	//bool inStore = false;
	bool inFightingArea = false;
	bool isFinishedShopping = false;
public:
	FVector FightingArea;
	UPROPERTY(EditAnywhere)
	float DistanceToStoreLocation = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "yoo")
	UFighterComponent* FighterComponent;
private:

public:
	AAIFighterCustomer();
	
	void UpdateBlackboard();
	void CheckDoneShopping();
	void IsInFightingArea();
	void CheckStartShopping();
	void CheckIsInFighterArea();
	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
};
