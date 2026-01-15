// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Push.h"
#include "AIcontroller.h"
#include "Push_Enemy.h"


UBTTask_Push::UBTTask_Push()
{
    NodeName = "Push";
    bCreateNodeInstance = true; 
}

EBTNodeResult::Type UBTTask_Push::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    APush_Enemy * Enemy = Cast <APush_Enemy> (OwnerComp.GetAIOwner()->GetPawn());

    if(Enemy == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    Enemy->Push();

    return EBTNodeResult::Succeeded;
}

