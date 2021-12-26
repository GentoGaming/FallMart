#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "PlayerComponents/PlayerGrabComponent.h"
#include "Team5_GP3/AI/AICustomerAbstract.h"

#include "PlayerCharacter.generated.h"

class UPhysicsHandleComponent;
class AThrowableItem;
class UBoxComponent;
class USplineMeshComponent;
class USplineComponent;
class UPlayerThrowRollComponent;
class UCharacterMovementComponent;
class AController;
class UCapsuleComponent;
class UPlayerMaskSlapComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMouseLeftKlickDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowingDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FThrowingReleasedDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FThrowingCustomerReleasedDelegate, AAICustomerAbstract *, AICustomer, UPhysicsHandleComponent* ,PhysicsHandleComponent);

DECLARE_DELEGATE_OneParam(FCustomInputDelegate, const bool);


UCLASS(Blueprintable, BlueprintType)
class TEAM5_GP3_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	FVector InputVector;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;
	UPROPERTY()
	APlayerController* playerController;
	float maskSlapCooldownCounter;
	UPROPERTY()
	UPlayerMaskSlapComponent* MaskSlapComponent;


private:
	void MoveForward(float input);
	void MoveRight(float input);
	void RotatePlayerCharacter(bool isSlaping);
	void ThrowingRollReleased();
	void DestroyInfected();
	void GrabTarget();

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UPlayerGrabComponent *PlayerGrabComponent;
	UPROPERTY(VisibleAnywhere)
	UPhysicsHandleComponent *PhysicsHandle;
	UFUNCTION(BlueprintImplementableEvent)
	void OnCustomerLanded(AAICustomerAbstract* CustomerAbs);
	UPROPERTY(BlueprintAssignable)
	FMouseLeftKlickDelegate OnLeftMouseClick;
	FThrowingDelegate OnThrow;
	FThrowingReleasedDelegate OnThrowReleased;
	FThrowingCustomerReleasedDelegate OnCustomerReleased;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Hair;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanMove;

	UPROPERTY(EditAnywhere, meta = (Category = "Attack | MaskSlapping"))
	UCapsuleComponent* SlapMaskCapsuleCollider;
	UPROPERTY(EditAnywhere, Category = "Attack | MaskSlapping")
	float MaskSlapCooldown;
	UPROPERTY(VisibleAnywhere, Category = "Attack | MaskSlapping")
	bool doMaskSlap;
	UPROPERTY(VisibleAnywhere, Category = "Attack | MaskSlapping")
	TSet<AActor*> overlappingSlapActors;
	UFUNCTION(BlueprintImplementableEvent)
	void OnGrabSuccess();
	UPROPERTY(EditAnywhere, Category="Player Infection Settings")
	float destroyInfectionRadius = 300.f;



	
	// Begin Of Roll
	// Player Hand.
	UPROPERTY(VisibleAnywhere)
	USceneComponent* PlayerHand;
	// rounded Ground area of roll hit
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SphereMesh;
	// Roll Spline Line towards the target
	UPROPERTY(VisibleAnywhere)
	USplineComponent* C_Spline;
	// Single Mesh make the line projectory
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	UStaticMeshComponent* SingleMesh;
	// End of roll	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Sets default values for this character's properties
	APlayerCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
