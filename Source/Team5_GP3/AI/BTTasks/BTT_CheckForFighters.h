// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_CheckForFighters.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTT_CheckForFighters : public UBTTaskNode
{
	GENERATED_BODY()
private:
	
	
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

