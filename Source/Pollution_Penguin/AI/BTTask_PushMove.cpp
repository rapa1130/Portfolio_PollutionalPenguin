// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_PushMove.h"
#include "AIcontroller.h"
#include "Push_Enemy2.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "AIUtility.h"

//Z값 안주고 그냥 미는 코드 

UBTTask_PushMove::UBTTask_PushMove()
{
    bNotifyTick = true; //TickTask 사용이 가능해짐! 
    bCreateNodeInstance = true; //인스턴스마다 노드 생성하도록 함! 
    NodeName = "PushMove";

}


EBTNodeResult::Type UBTTask_PushMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    if (OwnerComp.GetAIOwner() == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    Enemy = Cast <APush_Enemy2> (AIController->GetPawn());

    if(Enemy == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    Location = Enemy->GetActorLocation();
	End = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
    Direction = (End - Location).GetSafeNormal();

    //CheckForHit(OwnerComp);
    return EBTNodeResult::InProgress;
}

void UBTTask_PushMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    //바닥에 Hit이 없을경우 Task 종료 
    if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isMoving")))
    {
        // if (WalkSoundComponent)
        // {
        //     WalkSoundComponent->Stop();
        // }

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
        return;
    }
    
    //움직여!
    else if(OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isMoving"))){	//움직이는 중에만 
		MoveActor(Enemy, Direction, Enemy->Speed, true, DeltaSeconds);


        // if (!WalkSoundComponent)
        // {
        //     WalkSoundComponent = UGameplayStatics::SpawnSoundAttached(WalkSound, Enemy->GetRootComponent());
        // }
	}
}

