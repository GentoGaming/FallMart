// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_RunAway.h"

#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/AI/Customer Variation/AIAntiMaskerCustomer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_RunAway::UBTService_RunAway()
{
	NodeName = "Update RunAway Service";
}

void UBTService_RunAway::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIAntiMaskerCustomer* AICustomer = Cast<AAIAntiMaskerCustomer>(OwnerComp.GetOwner()->GetInstigator());
	if(AICustomer != nullptr)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("RunAway"), true);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("FleePlayerLocation"), AICustomer->FleeVectorLocation);
	}
}
