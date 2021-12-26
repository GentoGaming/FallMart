// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateFighterPartnerLocation.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTS_UpdateFighterPartnerLocation::UBTS_UpdateFighterPartnerLocation()
{
	NodeName = "BTS_Update fighter partner location";
}

void UBTS_UpdateFighterPartnerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                                 float DeltaSeconds)
{
	UFighterComponent* otherFighterComponent = Cast<UFighterComponent>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject("FightingPartner"));
	if(otherFighterComponent == nullptr) return;
	const FVector otherFighterLocation = otherFighterComponent->GetOwner()->GetActorLocation();
 	OwnerComp.GetBlackboardComponent()->SetValueAsVector("FighterPartnerLocation", otherFighterLocation) ;

}
