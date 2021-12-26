// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ItemLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"

UBTService_ItemLocation::UBTService_ItemLocation()
{
	NodeName = "Update Store Item Location";
}

void UBTService_ItemLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAICustomerAbstract* AICustomer = Cast<AAICustomerAbstract>(OwnerComp.GetOwner()->GetInstigator());
	if(AICustomer != nullptr && AICustomer->GetStoreItem() !=nullptr)
	{
		UE_LOG(LogTemp,Warning,TEXT("It is a customer"));
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(),AICustomer->GetStoreItem()->GetActorLocation());
	}
}
