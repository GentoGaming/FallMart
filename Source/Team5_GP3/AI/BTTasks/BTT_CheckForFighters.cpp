// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CheckForFighters.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team5_GP3/AI/FighterComponent.h"
#include "Team5_GP3/AI/Customer Variation/AIFighterCustomer.h"

EBTNodeResult::Type UBTT_CheckForFighters::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent) return EBTNodeResult::Failed;

	AAIFighterCustomer* AIFighter = Cast<AAIFighterCustomer>(OwnerComp.GetAIOwner()->GetPawn());
	UFighterComponent* thisFighterComponent = Cast<UFighterComponent>(
		AIFighter->GetComponentByClass(UFighterComponent::StaticClass()));
	UFighterComponent* otherFightingComponent = thisFighterComponent->GetFightingPartner();

	if (thisFighterComponent->GetFightingPartner() == nullptr || otherFightingComponent == nullptr) return EBTNodeResult::Failed;;
	
	otherFightingComponent->OnFighting.Broadcast();
	thisFighterComponent->OnFighting.Broadcast();
	
	Cast<AAIController>(otherFightingComponent->GetOwner()->GetOwner())->GetBlackboardComponent()->SetValueAsBool("IsFighting", true);
	BlackboardComponent->SetValueAsBool("IsFighting", true);

	return EBTNodeResult::Succeeded;
	// UFighterComponent* otherFigther = Cast<UFighterComponent>( BlackboardComponent->GetValueAsObject("FightingPartner"));
}
