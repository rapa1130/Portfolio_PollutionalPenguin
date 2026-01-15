// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Push.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTTask_Push : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Push();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
};
