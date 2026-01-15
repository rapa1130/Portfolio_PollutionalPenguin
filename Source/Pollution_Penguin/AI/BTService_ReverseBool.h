// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_ReverseBool.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API UBTService_ReverseBool : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTService_ReverseBool();
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


};
