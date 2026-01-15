// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GlacierSizeUp.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GlacierPlatform.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"




UBTTask_GlacierSizeUp::UBTTask_GlacierSizeUp()
{
    NodeName = "GlacierSizeUp";
}

EBTNodeResult::Type UBTTask_GlacierSizeUp::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);
    float size = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("Size");

    if (size == 0 || size > 3000){
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsOfClass(OwnerComp.GetWorld(), AGlacierPlatform::StaticClass(), FoundActors);

        // FoundActors 배열의 크기를 확인
        bool bIsPlatformPresent = FoundActors.Num() > 0;
        if (!bIsPlatformPresent)
        {
            return InitSize(size, OwnerComp);
        }
    }
    if (size > 10000){
        return InitSize(size, OwnerComp);
    }

    size += 500;
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Size", size);

    

    return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_GlacierSizeUp::InitSize(float &size, UBehaviorTreeComponent &OwnerComp)
{
    size = 0;
    OwnerComp.GetBlackboardComponent()->SetValueAsFloat("Size", size);
    OwnerComp.GetWorld()->GetTimerManager().SetTimer(TimerHandle, 5.0f, false); //과부하 방지용 
    return EBTNodeResult::Failed;
}
