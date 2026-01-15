// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "Squid.h"
#include "Map/GlacierPlatform.h"
#include "Components/BoxComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


#include "BTTask_SquidMove.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTTask_SquidMove : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SquidMove();

	UPROPERTY()
    UBoxComponent* CollisionComponent;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	ASquid * Squid;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	void HandleGlacierCollision(AGlacierPlatform* Glacier);


	FTimerHandle HitCooldownTimer;
	void ResetHitFlag();

	UFUNCTION()
	void InitializeCollisionComponent();
	void MoveTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetGlacier, float DeltaSeconds);


	UPROPERTY(EditAnywhere)
	USoundBase * GnawEfeect;




	

private:
	bool isMoving = true;
	bool bCanHit = true;
	FVector Direction;
	UBlackboardComponent* BlackboardComp;
	TArray<AActor*> OverlappedActors; 
	FVector GlacierLocation;
	FVector Velocity = FVector::ZeroVector;


	

};
