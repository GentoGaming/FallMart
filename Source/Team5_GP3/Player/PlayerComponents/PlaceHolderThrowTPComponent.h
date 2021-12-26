// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Team5_GP3/AI/Others/ThrowableItem.h"
#include "Team5_GP3/Player/PlaceHolderToiletPaperRoll.h"

#include "PlaceHolderThrowTPComponent.generated.h"


class USplineMeshComponent;
class APlayerCharacter;
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TEAM5_GP3_API UPlaceHolderThrowTPComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	APlayerCharacter* player;
public:
	UPROPERTY(EditAnywhere)
	float speed;
	UPROPERTY(EditAnywhere)
	float SphereTraceRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Casting Roll Settings")
	TSubclassOf<APlaceHolderToiletPaperRoll> ToiletPaperToThrow;
private:
protected:
	virtual void BeginPlay() override;

public:
	UPlaceHolderThrowTPComponent();
	UFUNCTION()
	void SpawnToiletPaper();
};
