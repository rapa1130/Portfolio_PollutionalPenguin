// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"
#include "ShooterCharacter.h"
#include "AIUtility.h"

//몬스터가 플레이어 위치 딱 포착하고, 해당 방향으로 돌아서는 코드 

UBTTask_ChasePlayer::UBTTask_ChasePlayer()
{
    NodeName = "ChasePlayer";
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){
    Super::ExecuteTask(OwnerComp, NodeMemory);

    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AShooterCharacter::StaticClass(), PlayerActors);
    
    //PlayerAActor를 ShooterCharacter로 캐스팅
    //캐스팅 했을때 isDead == true면 해당 배열에서 삭제 
    for (int32 i = PlayerActors.Num() - 1; i >= 0; --i) {
        AShooterCharacter* PlayerCharacter = Cast<AShooterCharacter>(PlayerActors[i]);
        if (PlayerCharacter && PlayerCharacter->isDead) {
            PlayerActors.RemoveAt(i);
        }
    }

    ClosestPlayer = FindClosestTarget(PlayerActors, OwnerComp.GetOwner()->GetActorLocation()); //가장 가까운 플레이어 찾기

    if (ClosestPlayer == nullptr){
        return EBTNodeResult::Failed;
    }

    //블랙보드 값 플레이어 위치, 부울값 업데이트 
    OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), ClosestPlayer -> GetActorLocation());
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("isMoving"), true); 
    OwnerComp.GetBlackboardComponent()->SetValueAsBool(TEXT("isNotSwimming"), false);
    OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), ClosestPlayer);

    RotateToActor(OwnerComp, ClosestPlayer); //플레이어 방향으로 회전
    AAIController* Controller = OwnerComp.GetAIOwner();
    Controller->ClearFocus(EAIFocusPriority::Gameplay); //이 노드 끝나고도 플레이어 바라보는 버그 수정 

    
    return EBTNodeResult::Succeeded;
}

