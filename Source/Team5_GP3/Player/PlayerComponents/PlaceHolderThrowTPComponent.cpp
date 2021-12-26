// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceHolderThrowTPComponent.h"

#include "Team5_GP3/Player/PlaceHolderToiletPaperRoll.h"
#include "Team5_GP3/Player/PlayerCharacter.h"

// Sets default values for this component's properties
UPlaceHolderThrowTPComponent::UPlaceHolderThrowTPComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}
// Called when the game starts
void UPlaceHolderThrowTPComponent::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<APlayerCharacter>(GetOwner());
	player->OnThrow.AddDynamic(this,&UPlaceHolderThrowTPComponent::SpawnToiletPaper);
	// 	Player->OnRollThrow.AddDynamic(this, &UPlayerThrowRollComponent::DrawProjectory);

	// ...
	
}



void UPlaceHolderThrowTPComponent::SpawnToiletPaper()
{
	FVector Location = player->PlayerHand->GetComponentLocation();
	FRotator Rotation = player->GetActorRotation();
	FActorSpawnParameters spawnInfo;
	spawnInfo.Owner = player;
	
	APlaceHolderToiletPaperRoll* TP = GetWorld()->SpawnActor<APlaceHolderToiletPaperRoll>(ToiletPaperToThrow,Location,Rotation,spawnInfo);
	


}





