// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "Team5_GP3/AI/FighterComponent.h"

#include "BTS_UpdateFighterPartnerLocation.generated.h"

/**
 * 
 */
UCLASS()
class TEAM5_GP3_API UBTS_UpdateFighterPartnerLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	private:
	UFighterComponent* FighterComponent;
	public:

	UBTS_UpdateFighterPartnerLocation();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
