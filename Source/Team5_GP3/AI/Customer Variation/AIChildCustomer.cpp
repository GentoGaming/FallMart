// Fill out your copyright notice in the Description page of Project Settings.


#include "AIChildCustomer.h"

#include "AIFighterCustomer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Team5_GP3/AI/CustomerComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


void AAIChildCustomer::BeginPlay()
{
	Super::BeginPlay();
	Player  = UGameplayStatics::GetPlayerPawn(this, 0);
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIChildCustomer::OnOverlapBegin);
	StopAttacking();
	
	GetCharacterMovement()->MaxWalkSpeed = childWalkSpeed;
}
void AAIChildCustomer::StopAttacking()
{
		// Called Once At BeginPlay and another time when child loose the target
		CustomerToAttack = nullptr;
		BlackboardComponent->ClearValue(TEXT("ChildTargetLocation"));
		GetCharacterMovement()->MaxWalkSpeed = childWalkSpeed;
	ToggleAndFireEvent(false);


}
void AAIChildCustomer::Tick(float DeltaSeconds)
{
	Super::Super::Tick(DeltaSeconds);

	// Run Away from Player is Postponed until functionality gets bit better . 
	// 
	/*------------------------------------------------------------------------------------------------------------*/
	/*
	if(CustomerToAttack!=nullptr || pathBlocked == true)
	{
		AvoidPlayer();
	}

	FHitResult hit;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), GetActorLocation(),
		(GetActorForwardVector() * FleeFromPlayerDistance) + GetActorLocation(), capsuleRadius,
		ETraceTypeQuery::TraceTypeQuery1, false, ignoreActors,
		EDrawDebugTrace::ForOneFrame, hit, true, FLinearColor::Gray);
	if (hit.bBlockingHit)
	{
		pathBlocked = true;
		FleeVectorLocation = GetActorRotation().GetInverse().Vector();
		FindValidLocation(FleeVectorLocation);
		GetMovementComponent()->StopMovementImmediately();
		BlackboardComponent->SetValueAsVector(TEXT("FleePlayerLocation"), FleeVectorLocation);
		RunAwayFromPlayer(pathBlocked);
	}
	if (pathBlocked)
	{
		if (FVector::Dist(GetActorLocation(), FleeVectorLocation) < AllowedDistanceFromFleePlayerLocation) pathBlocked = false;
		BlackboardComponent->ClearValue(TEXT("ParentLocation"));
		BlackboardComponent->ClearValue(TEXT("ChildTargetLocation"));
		BlackboardComponent->SetValueAsBool(TEXT("PathBlocked"), pathBlocked);
		return ; // Quit As We don't want to do AnyTask Other than Flee from player .
	}else
	{
		BlackboardComponent->ClearValue(TEXT("PathBlocked"));	
	}
	*/	
/*-----------------------------------------------------------------------------------------------------------*/

	if(childIsDone) return;
	
	
	if(Guardian!=nullptr && !Guardian->IsActorBeingDestroyed() && (CustomerToAttack == nullptr || CustomerToAttack->IsActorBeingDestroyed()))
	{
		isStarted = true;
		BlackboardComponent->SetValueAsVector(TEXT("ParentLocation"), Guardian->GetActorLocation());
		GetCharacterMovement()->MaxWalkSpeed = childWalkSpeed;
		ToggleAndFireEvent(false);

	}else
	{
		BlackboardComponent->ClearValue(TEXT("ParentLocation"));
	}

	if(CustomerToAttack!= nullptr && !CustomerToAttack->IsActorBeingDestroyed())
	{
		BlackboardComponent->ClearValue(TEXT("ParentLocation"));
		BlackboardComponent->SetValueAsVector(TEXT("ChildTargetLocation"), CustomerToAttack->GetActorLocation());
		const bool rdyToTakeMaskOff = FVector::PointsAreNear(GetActorLocation(), CustomerToAttack->GetActorLocation(), takeMaskFromDistance);
		if(rdyToTakeMaskOff)
		{
			// When Child Is Next to Target , Takes Off Mask, Then Goes back to Parent if he still in supermarket , else he will leave SuperMarket
			CustomerToAttack->CustomerComponent->TakeOffMask();
			StopAttacking();

		}
	}else
	{
		StopAttacking();
	}


	CheckIfParentLeftStore();

}

void AAIChildCustomer::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	if(CustomerToAttack == nullptr && Guardian!=nullptr && !childGotThrown)
	{
		
		//const bool isChildNextToParent = FVector::PointsAreNear(GetActorLocation(), Guardian->GetActorLocation(), childDistanceFromParent);

		const bool isChild = OtherActor->IsA(AAIChildCustomer::StaticClass());
		const bool isFighter = OtherActor->IsA(AAIFighterCustomer::StaticClass());
		const bool isPlayer = OtherActor == Player;
		const bool isSameCustomer = (CustomerToAttack == OtherActor);
		const bool isGuardian = OtherActor == Guardian;

		for(AActor *Actor : AllActorsTriedToAttack)
		{
			if(OtherActor == Actor) return;
		}
		
		if(isChild || isPlayer || isSameCustomer|| isFighter || isGuardian) return;
		
		
		/* Be sure its a Customer ( Basically its 100% sure its a customer but checking is a good thing to do here )
		Checking Also if the customer have a mask . If yes than we exit , as we want the customer to has mask
		Or if another child is attacking this same Customer so we dont do it .
		So We Take it off */
		
		AAICustomerAbstract* CustomerPassingBy =  Cast<AAICustomerAbstract>(OtherActor);
		if(CustomerPassingBy == nullptr || !CustomerPassingBy->CustomerComponent->HasMask || CustomerComponent->gettingAttackedByAChild) return;
		const int16 ChildCrazyProb =  FMath::RandRange(0, 100);
		if(ChildCrazyProb > GameManager->childCrazinessProbability)
		{
			AllActorsTriedToAttack.Add(CustomerPassingBy);
			return;
		}

		// So if probability We execute the following 
		
		// Found the Customer
		CustomerPassingBy->CustomerComponent->gettingAttackedByAChild = true;
		CustomerToAttack = 	CustomerPassingBy;
		GetCharacterMovement()->MaxWalkSpeed = childChaseSpeed;
		ToggleAndFireEvent(true);

	}	
}

AAIChildCustomer::AAIChildCustomer()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Child Vision"));
	BoxComponent->SetupAttachment(RootComponent);
}

void AAIChildCustomer::CheckIfParentLeftStore()
{
	if(Guardian!= nullptr && Guardian->IsActorBeingDestroyed() && isStarted)
	{
		// Child Parent has destroyed itself outside the supermarket and therefore the child will follow to the exit and destroySelf too .
		// Will finish his task unless player stopped him .
		childIsDone = true;
		GetCharacterMovement()->MaxWalkSpeed = childWalkSpeed;
		ToggleAndFireEvent(false);
		BlackboardComponent->ClearValue(TEXT("ParentLocation"));
		BlackboardComponent->ClearValue(TEXT("ChildTargetLocation"));
		BlackboardComponent->SetValueAsBool(TEXT("ChildShouldLeaveStore"), true);
	}
}

void AAIChildCustomer::ToggleAndFireEvent(bool condition)
{
if(condition != isCrazy) OnChildCraziness(condition);

	isCrazy = condition;

}
