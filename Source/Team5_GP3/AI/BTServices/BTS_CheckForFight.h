// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckForFight.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTS_CheckForFight : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	private:
	TArray<UPrimitiveComponent*> OutComponents;
	
	UPROPERTY(EditAnywhere)
	float SearchRange = 200.f;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeList;
	// UPROPERTY(EditAnywhere)
	// TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditAnywhere)
	TSubclassOf<APawn> ClassFilter;

	private:

	UBTS_CheckForFight();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

