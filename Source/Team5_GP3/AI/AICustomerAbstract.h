// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AICustomerController.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "Others/StoreItem.h"
#include "AICustomerAbstract.generated.h"
class UProjectileMovementComponent;
class UCustomerComponent;
class UGameManager;
class UBlackboardComponent;
class UBehaviorTree;
class AAIController;

UCLASS(Abstract)
class TEAM5_GP3_API AAICustomerAbstract : public ACharacter
{
	GENERATED_BODY()


public:
	AAICustomerAbstract();

	virtual void Tick(float DeltaSeconds) override;
	virtual void BeginPlay() override;
	UPROPERTY()
	AAICustomerController* AICustomController;
	UPROPERTY(EditAnywhere, meta =(Category = "Shopping | Shopping"))
	int itemsToShop = 3;
	UPROPERTY(EditAnywhere, Category = "Shopping | Movement")
	float AllowedEntranceDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping | Movement")
	float DistanceToShopItem;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shopping|Movement")
	float AllowedDistanceToGoal = 200.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Debug")
	bool pathBlocked = false;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
	bool inStore = false;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
	FVector FleeVectorLocation;
	UPROPERTY()
	TArray<AActor*> ignoreActors;
	UPROPERTY(EditAnywhere)
	UBehaviorTree* AIBehavior;
	UPROPERTY()
	FVector StoreEntryPoint;
	UPROPERTY()
	UGameManager* GameManager;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
	AStoreItem* StoreItemToShop;
	UPROPERTY(VisibleAnywhere, Category = "Debug")
	TArray<AStoreItem*> AllPickedItems;
	UPROPERTY(BlueprintReadWrite)
	FVector ItemLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MakeEditWidget))
	TArray<FTransform> CarriedItemLocations;
	UFUNCTION(BlueprintCallable)
	AStoreItem* GetStoreItem();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Outfit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* MaskMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMeshComponent* Hand;
	UFUNCTION(BlueprintCallable)
	bool IsCarryingAnItem();
	UFUNCTION(BlueprintImplementableEvent)
	void OnInfected(bool Cond);
	
	UFUNCTION(BlueprintCallable)
	bool SetStoreItem();
	UFUNCTION(BlueprintCallable)
	TArray<AStoreItem*> GetAllPickedItems() const { return AllPickedItems; };
	UFUNCTION(BlueprintCallable)
	UBehaviorTree* GetAIBehavior() const { return AIBehavior; };
	UFUNCTION(BlueprintCallable)
	// Direction
	float GetPlayerDirection() const;
	UFUNCTION(BlueprintCallable, meta =(ToolTip = "The index is for the Carried Item Locations"))
	FTransform GetCarriedItemLocation(int Index);
	UFUNCTION(BlueprintCallable)
	void DestroySelf();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCustomerComponent* CustomerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* CloneMeshComponent;
	UFUNCTION(BlueprintCallable)
	void ResumeLogic() { AICustomController->GetBrainComponent()->ResumeLogic("Customer Landed"); };
	UFUNCTION(BlueprintCallable)
	void AddToAllPickedItems(AStoreItem* Item) ;
	UFUNCTION(BlueprintCallable)
	void DeleteAllItems();
	UFUNCTION(BlueprintImplementableEvent)
	void PutClothOn();
protected:
	UPROPERTY()
	UBlackboardComponent* BlackboardComponent;
	void SwapItems(AStoreItem *FirstItem, AStoreItem *SecondItem);
	bool isMapLoaded = false;
	void InitCustomer();
	void UpdateCustomerBlackboard();
	void SetCustomerRotation();
	FVector CapsuleLocation;
	FVector MeshLocation;
	FVector HighUpLocation = {50000, 50000, 50000};
};
