// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"


UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
    NodeName = "Update Player Location If Seen";
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn * PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); //플레이어의 폰을 얻는다.
    if (PlayerPawn == nullptr){
        return;
    }

    if (OwnerComp.GetAIOwner() == nullptr) //폰을 얻고 폰을 슈터 캐릭터로 보냄 
    {
        return;
    }

    if (OwnerComp.GetAIOwner()->LineOfSightTo(PlayerPawn)) 
    {
        //플레이어 위치 설정
        OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn -> GetActorLocation()); //블랙보드에 플레이어의 위치를 저장한다.
    }
    else    //시야 미확보시 블랙보드에서 해당 키값을 지운다!
    {
        //OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnwonPlayerLocation"), PlayerPawn -> GetActorLocation()); //블랙보드에 플레이어의 위치를 저장한다.
        OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn -> GetActorLocation()); //블랙보드에 플레이어의 위치를 저장한다.
}

