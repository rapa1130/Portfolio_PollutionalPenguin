// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Squid.h"
#include "Components/SphereComponent.h"
#include "GlacierPlatform.h"

#include "BTTask_CollisionSphere.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTTask_CollisionSphere : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_CollisionSphere();

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	class USphereComponent* CollisionSphere;

private: 
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void InitializeCollisionSphere();

	void ResetGlacierList();

	void SetupCollisionSphere(APawn* OwnerActor, float GlacierSize);
	
	void CleanupCollisionSphere();

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AActor*> GlacierList;
	AActor* ClosestGlacier = nullptr;

};
