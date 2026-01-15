// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "AIcontroller.h"
#include "ShooterCharacter.h"


UBTTask_Shoot::UBTTask_Shoot()
{
    NodeName = "Shoot";
}


EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    
    if (OwnerComp.GetAIOwner() == nullptr) //폰을 얻고 폰을 슈터 캐릭터로 보냄 
    {
        return EBTNodeResult::Failed;
    }

    AShooterCharacter * Character = Cast <AShooterCharacter> (OwnerComp.GetAIOwner()->GetPawn()); //슈터 캐릭터의 쏘는 함수를 실행.
    
    if (Character == nullptr)
    {
        return EBTNodeResult::Failed;
    }

    Character -> Shoot();

    return EBTNodeResult::Succeeded;
}
