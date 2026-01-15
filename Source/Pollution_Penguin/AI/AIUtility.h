#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


AActor* FindClosestTarget(const TArray<AActor*>& Targets, const FVector& Location);


void RotateToActor(UBehaviorTreeComponent& OwnerComp, AActor* Target, float DeltaSeconds = 0.0f);

void MoveActor(AActor* Actor, const FVector& Direction, float Speed, bool bKeepZ, float DeltaSeconds);
void MoveActorWithInertia(AActor* Actor, const FVector& Direction, float & Speed, bool bKeepZ, float DeltaSeconds);
