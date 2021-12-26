// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckForFight.h"

#include <string>


#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Team5_GP3/AI/Customer Variation/AIFighterCustomer.h"

UBTS_CheckForFight::UBTS_CheckForFight()
{
	NodeName = "BTS_CheckForFight";
}

void UBTS_CheckForFight::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// UE_LOG(LogTemp,Display,TEXT("%f"), DeltaSeconds)
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (BlackboardComponent->GetValueAsObject("FightingPartner") != nullptr) return;

	TArray<AActor*> ActorsToIgnore;
	AAIFighterCustomer* AIFighter = Cast<AAIFighterCustomer>(OwnerComp.GetAIOwner()->GetPawn());
	ActorsToIgnore.Add(AIFighter);
	ActorsToIgnore.Add(Cast<AAIFighterCustomer>(this));

	TArray<AActor*> OutActors;


	if (UKismetSystemLibrary::SphereOverlapActors(GetWorld(), AIFighter->GetActorLocation(), SearchRange,
	                                              ObjectTypeList, ClassFilter, ActorsToIgnore, OutActors))
	{

		for (auto Actor : OutActors)
		{
			UBlackboardComponent* OtherBlackBoardComponent = Cast<AAIController>(Actor->GetOwner())->
            GetBlackboardComponent();
			if (BlackboardComponent == nullptr || OtherBlackBoardComponent == nullptr) continue;


			UFighterComponent* thisFighterComponent = Cast<UFighterComponent>(
                AIFighter->GetComponentByClass(UFighterComponent::StaticClass()));
			UFighterComponent* otherFightingComponent = Cast<UFighterComponent>(
                Actor->GetComponentByClass(UFighterComponent::StaticClass()));

			if (thisFighterComponent == nullptr || otherFightingComponent == nullptr || OtherBlackBoardComponent->
                GetValueAsBool("IsFighting") || OtherBlackBoardComponent->GetValueAsObject("FightingPartner") != nullptr)
                // otherFightingComponent->GetFightingPartner() != nullptr ||thisFighterComponent->FightingPartner != nullptr ||
                // !OtherBlackBoardComponent->GetValueAsBool("FinnishedShopping"))
			{
				continue;

			}
	

			thisFighterComponent->SetFightingPartner(
                Cast<UFighterComponent>(Actor->GetComponentByClass(UFighterComponent::StaticClass())));
			otherFightingComponent->SetFightingPartner(thisFighterComponent);

			BlackboardComponent->SetValueAsObject("FightingPartner", otherFightingComponent);

			Cast<AAIController>(Actor->GetOwner())->GetBlackboardComponent()->SetValueAsObject(
                "FightingPartner", thisFighterComponent);
			UE_LOG(LogTemp, Display, TEXT("s% is fighting s%"), *AIFighter->GetName(), *Actor->GetName())
			return;
		}

		// if (BlackboardComponent->GetValueAsObject("FightingPartner") == thisFighterComponent)
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("fighting partner is this"));
		// }
		//
		// if (thisFighterComponent == otherFightingComponent)
		// {
		// 	UE_LOG(LogTemp, Display, TEXT("fighting partner is this2"));
		// }
		// UE_LOG(LogTemp, Display, TEXT("%s is fighting %s"), *AIFighter->GetName(), *OutActors[0]->GetName())
	}


	// else
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("faild to overlap %s"), *AIFighter->GetActorLocation().ToString())
	// 	DrawDebugSphere(GetWorld(), AIFighter->GetActorLocation(), SearchRange, 8, FColor::Red, false, 1.f);
	// }
}



//
// 		
// UBlackboardComponent* OtherBlackBoardComponent = Cast<AAIController>(OutActors[0]->GetOwner())->
//     GetBlackboardComponent();
// if (BlackboardComponent == nullptr || OtherBlackBoardComponent == nullptr) return;
//
//
// UFighterComponent* thisFighterComponent = Cast<UFighterComponent>(
//     AIFighter->GetComponentByClass(UFighterComponent::StaticClass()));
// UFighterComponent* otherFightingComponent = Cast<UFighterComponent>(
//     OutActors[0]->GetComponentByClass(UFighterComponent::StaticClass()));
//
// if (thisFighterComponent == nullptr || otherFightingComponent == nullptr || OtherBlackBoardComponent->
//     GetValueAsBool("IsFighting") || OtherBlackBoardComponent->GetValueAsObject("FightingPartner") != nullptr)
//     	// otherFightingComponent->GetFightingPartner() != nullptr || thisFighterComponent->FightingPartner != nullptr
//     	// ||			!OtherBlackBoardComponent->GetValueAsBool("FinnishedShopping"))
//     	return;
//
// thisFighterComponent->SetFightingPartner(
//     Cast<UFighterComponent>(OutActors[0]->GetComponentByClass(UFighterComponent::StaticClass())));
// otherFightingComponent->SetFightingPartner(thisFighterComponent);
//
// BlackboardComponent->SetValueAsObject("FightingPartner", otherFightingComponent);
//
// OtherBlackBoardComponent->SetValueAsObject(
//     "FightingPartner", thisFighterComponent);
//
// // BlackboardComponent->SetValueAsBool("IsFighting", true);
// // OtherBlackBoardComponent->SetValueAsBool(
// // 	"IsFighting", true);
