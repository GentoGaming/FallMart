// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceShopItemLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTServiceShopItemLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
		UBTServiceShopItemLocation();
		virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime) override;
};
