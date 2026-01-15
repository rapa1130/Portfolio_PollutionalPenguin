// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_IsPlayerDie.h"
#include "GameFramework/Actor.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ShooterCharacter.h"


UBTService_IsPlayerDie::UBTService_IsPlayerDie()
{
    NodeName = "IsPlayerDie";
}


void UBTService_IsPlayerDie::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    AActor * Player = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("Player")));
    if (Player == nullptr) 
        return;


    AShooterCharacter * ShooterCharacter = Cast<AShooterCharacter>(Player);
    if (Player == nullptr) 
        return;


    if (ShooterCharacter->isDead == true)
    {
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false); 
    }
}

