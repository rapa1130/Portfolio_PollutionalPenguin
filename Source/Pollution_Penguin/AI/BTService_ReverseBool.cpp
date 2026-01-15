// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_ReverseBool.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"

UBTService_ReverseBool::UBTService_ReverseBool()
{
    NodeName = "Update Reverse Bool";
}

void UBTService_ReverseBool::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::OnBecomeRelevant(OwnerComp, NodeMemory);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (BlackboardComp)
    {
        BlackboardComp->SetValueAsBool(GetSelectedBlackboardKey(), true);

    }
}