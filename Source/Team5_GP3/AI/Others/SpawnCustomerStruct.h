// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "SpawnCustomerStruct.generated.h"


UENUM (BlueprintType)
enum class EDays : uint8
{
	D_ChooseADay UMETA(DisplayName="Choose a Day"),
	D_Monday UMETA(DisplayName="Monday"),
	D_Tuesday UMETA(DisplayName="Tuesday"),
	D_Wednesday UMETA(DisplayName="Wednesday"),
	D_Thursday UMETA(DisplayName="Thursday"),
	D_Friday UMETA(DisplayName="Friday"),
	D_Saturday UMETA(DisplayName="Saturday"),
	D_Sunday UMETA(DisplayName="Sunday"),
};

USTRUCT(BlueprintType)
struct  FSpawnCustomerStruct
{
	GENERATED_BODY()
	FSpawnCustomerStruct() = default;
	explicit  FORCEINLINE  FSpawnCustomerStruct (TSubclassOf<AAICustomerAbstract> AICustomer, TMap<EDays, int> DBProbability);
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AAICustomerAbstract> AICustomerToSpawn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EDays, int> DayBaseProbability;
};


FSpawnCustomerStruct::FSpawnCustomerStruct(TSubclassOf<AAICustomerAbstract> AICustomer, TMap<EDays, int> DBProbability)
{
	AICustomerToSpawn = AICustomer;
	DayBaseProbability = DBProbability;
}
