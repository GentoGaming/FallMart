// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_RunAway.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTService_RunAway : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	UBTService_RunAway();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
