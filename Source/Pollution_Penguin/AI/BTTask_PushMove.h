// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h" 
#include "Push_Enemy2.h" 
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h" 
#include "BTTask_PushMove.generated.h"


/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTTask_PushMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_PushMove();
	
private:	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	APush_Enemy2* Enemy; 
	FVector Direction;
	FVector Location;
	FVector End;

	UPROPERTY(EditAnywhere)
	USoundBase * WalkSound;

	UAudioComponent* WalkSoundComponent;
};
