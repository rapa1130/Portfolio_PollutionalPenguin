// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBTService_PlayerLocation::UBTService_PlayerLocation()
{
    NodeName = "Update Player Location";
}


void UBTService_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    DeltaSeconds = 0.5f; //0.5초마다 플레이어의 위치를 업데이트한다.
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    APawn * PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); //플레이어의 폰을 얻는다.
    if (PlayerPawn == nullptr){
        return;
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), PlayerPawn -> GetActorLocation()); //블랙보드에 플레이어의 위치를 저장한다.
}


