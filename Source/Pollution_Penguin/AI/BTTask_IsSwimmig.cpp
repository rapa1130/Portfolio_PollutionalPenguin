// // Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_IsSwimmig.h"
#include "AIcontroller.h"
#include "Push_Enemy2.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AIUtility.h"
#include "Kismet/GameplayStatics.h"



UBTTask_IsSwimmig::UBTTask_IsSwimmig()
{
    bNotifyTick = true; //TickTask 사용이 가능해짐! 
    bCreateNodeInstance = true; //인스턴스마다 노드 생성하도록 함! 
    NodeName = "IsSwimming";
}

EBTNodeResult::Type UBTTask_IsSwimmig::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

    if(OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isNotSwimming")) == true){
        return EBTNodeResult::Succeeded;
    }

    Location = Enemy->GetActorLocation();
	End = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TEXT("PlayerLocation"));
    Direction = (End - Location).GetSafeNormal();

    return EBTNodeResult::InProgress;

}



void UBTTask_IsSwimmig::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    //움직여!
    if(!OwnerComp.GetBlackboardComponent()->GetValueAsBool(TEXT("isNotSwimming"))){	//움직이는 중에만 
		MoveActor(Enemy, Direction, Enemy->Speed, false, DeltaSeconds);
        // if (!SwimSoundComponent)
        // {
        //     SwimSoundComponent = UGameplayStatics::SpawnSoundAttached(SwimSound, Enemy->GetRootComponent());
        // }
	}
    else{
        // if (SwimSoundComponent)
        // {
        //     SwimSoundComponent->Stop();
        // }
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded); 
    }

}

