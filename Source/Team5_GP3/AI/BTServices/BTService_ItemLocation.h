// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ItemLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTService_ItemLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	public:
	UBTService_ItemLocation();
	protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
