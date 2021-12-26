// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"
#include "Team5_GP3/Managers/GameManager.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
	NodeName = "Update Player Location";
}

void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAICustomerAbstract* AICustomer = Cast<AAICustomerAbstract>(OwnerComp.GetOwner()->GetInstigator());
	if(AICustomer != nullptr && AICustomer->GameManager!=nullptr)
	{
		 OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), AICustomer->GameManager->GetPlayerLocation());
	}
}
