// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"


#include "ThrowableItem.generated.h"
class UPhysicsHandleComponent;
class AAICustomerAbstract;
class APlayerCharacter;

UCLASS()
class TEAM5_GP3_API AThrowableItem : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<AActor*> OutActors;
public:
	UFUNCTION(BlueprintCallable)
	void HitTheGround();

	UFUNCTION(BlueprintCallable)
	void ZKillTriggered();
	
	UFUNCTION()
	void ThrowCustomer(const FVector& Vector, AAICustomerAbstract* CustomerToThrow, UPhysicsHandleComponent* PhysicsHandleComponent);
	UPROPERTY()
	APlayerCharacter* Player;
	UPROPERTY(EditAnywhere)
	float OverlapFighterRadius = 150.f;
	UPROPERTY(EditAnywhere)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypeList;
	UPROPERTY(EditAnywhere)
	TArray<AActor*> ActorsToIgnore;
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ClassFilter;
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent;
	UPROPERTY(EditAnywhere)
	bool isCustomerRoll = false;
	UFUNCTION(BlueprintPure)
	APlayerCharacter* GetPlayer() const {return Player;};

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	bool isThrowingACustomer = false;

	UPROPERTY()
	AAICustomerAbstract* CustomerThrowing;
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle;
	
public:
	// Sets default values for this actor's properties
	AThrowableItem();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHitGround();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void Throw(FVector Velocity);
	UFUNCTION(BlueprintCallable)
	void OverlapFighterOnCollision(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);
};
