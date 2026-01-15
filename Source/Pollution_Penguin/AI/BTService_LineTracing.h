// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_LineTracing.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTService_LineTracing : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_LineTracing();
	
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	void HandleHit(UBehaviorTreeComponent& OwnerComp, const TArray<FHitResult>& HitResults);
	FVector GetStartRay(const FRotator& EnemyRotation) const;

private:
	AActor* Enemy;
};
