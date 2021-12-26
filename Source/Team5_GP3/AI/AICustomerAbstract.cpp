// Fill out your copyright notice in the Description page of Project Settings.


#include "AICustomerAbstract.h"

#include <vcruntime_startup.h>
#include "Math/Transform.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "AICustomerController.h"
#include "CustomerComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Team5_GP3/Managers/GameManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AAICustomerAbstract::AAICustomerAbstract()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AICustomController = CreateDefaultSubobject<AAICustomerController>(TEXT("AICustomerControllerComponent"));
	CustomerComponent = CreateDefaultSubobject<UCustomerComponent>(TEXT("CustomerComponent"));
	Outfit = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Customers Outfit"));
	Outfit->SetupAttachment(GetMesh());
	Hair = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hair"));
	MaskMesh  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mask"));
	Hair->SetupAttachment(GetMesh());
	Hand  = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hand"));
	Hand->SetupAttachment(GetMesh());
	MaskMesh->SetupAttachment(GetMesh());
	
	GetCharacterMovement()->bUseRVOAvoidance = true;
	GetCharacterMovement()->AvoidanceConsiderationRadius = 500;
	if (CarriedItemLocations.Num() == 0)
	{
			CarriedItemLocations.Add(FTransform());
			CarriedItemLocations[0].SetLocation(FVector(0.f, -85.f, 0.f));
			CarriedItemLocations.Add(FTransform());
			CarriedItemLocations[1].SetLocation(FVector(0.f, 80.f, 0.f));
			CarriedItemLocations.Add(FTransform());
			CarriedItemLocations[2].SetLocation(FVector(0.f, -10.f, 150.f));
		
	}
}

void AAICustomerAbstract::Tick(float DeltaSeconds)
{
	if (isMapLoaded) return;
	UpdateCustomerBlackboard();
	// InitCustomer();
}

// Called when the game starts or when spawned
void AAICustomerAbstract::BeginPlay()
{
	Super::BeginPlay();
	CapsuleLocation = GetCapsuleComponent()->GetComponentLocation();
	MeshLocation = GetMesh()->GetComponentLocation();
	InitCustomer();
	CustomerComponent->ActivateThreat(GameManager);
	AICustomController->RunBehaviorTree(AIBehavior);
	Hair->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MaskMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Hand->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

AStoreItem* AAICustomerAbstract::GetStoreItem()
{
	return StoreItemToShop;
}

bool AAICustomerAbstract::IsCarryingAnItem() 
{
	if(AllPickedItems.Num() > 0)
	{
		Hand->SetVisibility(true);
		return true;
	}
	Hand->SetVisibility(false);
		return false;
}

bool AAICustomerAbstract::SetStoreItem()
{
	if (!isMapLoaded) return false;
	StoreItemToShop = GameManager->PopAStoreItem();
	if (StoreItemToShop == nullptr) return false;

//	AllPickedItems.Add(StoreItemToShop);

	return true;
}

float AAICustomerAbstract::GetPlayerDirection() const
{
	return GameManager->GetPlayerDirection(GetOwner());
}

void AAICustomerAbstract::DestroySelf()
{
	CustomerComponent->BeforeDestroyed();
	/*
	if (AllPickedItems.Num() > 0)
	{
		for (AStoreItem* StoreItem : GetAllPickedItems())
		{
			if (StoreItem != nullptr)
				StoreItem->Destroy();
		}
	}
	*/
	DeleteAllItems();
	GetInstigator()->Destroy();
}






 


void AAICustomerAbstract::AddToAllPickedItems(AStoreItem* Item)
{
	AllPickedItems.Add(Item);
	Hand->SetVisibility(true);
	TArray<AStoreItem*> AllItemsOrderBySize;
	FVector CurrentItem  = FVector::ZeroVector;
	FVector NextItem = FVector::ZeroVector;
	// Lets get All the items and check Which one is bigger
	const int ItemsCount = AllPickedItems.Num();
	

		int i, j, min_idx;
		// One by one move boundary of unsorted subarray
		for (i = 0; i < ItemsCount-1; i++)
		{
			// Find the minimum element in unsorted array
			min_idx = i;
			for (j = i+1; j < ItemsCount; j++)
			{
				if(AllPickedItems[min_idx]!=nullptr && AllPickedItems[j]!=nullptr){
					CurrentItem = AllPickedItems[min_idx]->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().GetSize();
					NextItem    = AllPickedItems[j]->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().GetSize();

					if (CurrentItem.X < NextItem.X || CurrentItem.Y < NextItem.Y)
					{
						min_idx = j;
					}
				}
				// Swap the found minimum element with the first element
				if(min_idx!=i)
				{
					SwapItems(AllPickedItems[min_idx], AllPickedItems[i]);
				}
			}
		}

	FVector shopItemSize = FVector::ZeroVector;
	FVector previousShopItemSize = shopItemSize;
	FVector shopItemPosition = FVector::ZeroVector;
	FVector previousShopItemPosition;
	FBox itemBoundingBox;
	FBox previousItemBoundingBox;
	for(AStoreItem *OneItem : AllPickedItems)
	{
		itemBoundingBox = itemBoundingBox + OneItem->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox();
		shopItemSize = shopItemSize + OneItem->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().GetSize();
		if (abs(previousShopItemSize.Z) > abs(previousShopItemSize.Y) || abs(previousShopItemSize.Z) > abs(previousShopItemSize.Y))
		{
			if (abs(previousShopItemSize.X) > abs(previousShopItemSize.Y))
				shopItemPosition.Y += abs(previousShopItemPosition.Y + previousItemBoundingBox.Max.Y);			
			else
				shopItemPosition.X += abs(previousShopItemSize.X+previousItemBoundingBox.Max.X);
		}
		else
		{
			shopItemPosition.X = 0;
			shopItemPosition.Y = 0;
			shopItemPosition.Z += abs(previousShopItemSize.Z + previousItemBoundingBox.Max.Z);
		}
		previousShopItemPosition = shopItemPosition;
		previousShopItemSize = shopItemSize;
		previousItemBoundingBox = itemBoundingBox;
		OneItem->SetActorLocation(Hand->GetComponentLocation()
			+ FVector(shopItemPosition.X + abs(OneItem->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().Min.X),
			shopItemPosition.Y + abs(OneItem->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().Min.Y),
				shopItemPosition.Z - 40 + abs(OneItem->GetStaticMeshComponent()->GetStaticMesh()->GetBoundingBox().Min.Z)));
	}	
	
}




void AAICustomerAbstract::DeleteAllItems()
{
	if (AllPickedItems.Num() > 0)
	{
	for (AStoreItem* StoreItem : GetAllPickedItems())
	{
	if (StoreItem != nullptr)
	StoreItem->Destroy();
	}
	}
	
	Hand->SetVisibility(false);
	AllPickedItems.Empty();
	
}


void AAICustomerAbstract::SwapItems(AStoreItem* FirstItem, AStoreItem* SecondItem)
{
	AStoreItem* TempItem = FirstItem;
	FirstItem = SecondItem;
	SecondItem = TempItem;
}

void AAICustomerAbstract::InitCustomer()
{
	GameManager = Cast<UGameManager>(GetGameInstance());
	if (!GetController()) SpawnDefaultController();

	AICustomController = Cast<AAICustomerController>(GetController());
	// if (GameManager == nullptr || AICustomController == nullptr) return;


	AICustomController->RunBehaviorTree(AIBehavior);
	GameManager->AddCustomers(this);
	StoreEntryPoint = GameManager->GetStoreEntryLocation();

	BlackboardComponent = AICustomController->GetBlackboardComponent();
	BlackboardComponent->SetValueAsVector(TEXT("PlayerLocation"), GameManager->GetPlayerLocation());
	BlackboardComponent->SetValueAsVector(TEXT("StoreEntryPoint"), GameManager->GetStoreEntryLocation());
	BlackboardComponent->SetValueAsVector(TEXT("StoreExitPoint"), GameManager->GetStoreExitLocation());
	BlackboardComponent->SetValueAsVector(TEXT("FightingAreaLocation"), GameManager->GetFightingAreaLocation());
	BlackboardComponent->SetValueAsFloat(TEXT("AllowedDistanceToGoal"), AllowedDistanceToGoal);
	// To choose a place to exit too when we are done . Will delete this code in case not needed . Keeping it for now just rdy but not used in a efficient way.
	const int SpawnCount = GameManager->GetSpawnLocationCount();
	if (SpawnCount > 0)
	{
		BlackboardComponent->SetValueAsVector(
			TEXT("SpawnLocation"), GameManager->GetSpawnLocations()[FMath::RandRange(0, SpawnCount - 1)]);
	}

	isMapLoaded = true;
}

void AAICustomerAbstract::UpdateCustomerBlackboard()
{
	BlackboardComponent->SetValueAsVector(TEXT("PlayerLocation"), GameManager->GetPlayerLocation());
}

void AAICustomerAbstract::SetCustomerRotation()
{
	FRotator rotation = FRotator::ZeroRotator;

	SetActorRotation(rotation);
}

FTransform AAICustomerAbstract::GetCarriedItemLocation(int Index)
{
	if (Index >= CarriedItemLocations.Num())
	{
		FTransform itemTransform = GetActorTransform();
		if (GetStoreItem())
		{
			itemTransform = itemTransform.GetRelativeTransform(GetStoreItem()->GetTransform());
		}
		CarriedItemLocations.Add(itemTransform);
		Index = CarriedItemLocations.Num() - 1;
	}
	return CarriedItemLocations[Index];
}
