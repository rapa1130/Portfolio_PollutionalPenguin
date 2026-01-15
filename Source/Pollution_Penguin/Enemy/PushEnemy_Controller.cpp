// Fill out your copyright notice in the Description page of Project Settings.


#include "PushEnemy_Controller.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"


void APushEnemy_Controller::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr){
        RunBehaviorTree(AIBehavior);   //BT 실행
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장

    }
}



void APushEnemy_Controller::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    //해당 코드는 BT_PlayerLocationIfSeen.cpp에 대체함. 
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 


    if(LineOfSightTo(PlayerPawn)){
        //플레이어 위치 설정
        //플레이어 마지막 위치도 설정 
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
        GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
    }
    else{
        //플레이어 위치 clear 
        GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    }
}
