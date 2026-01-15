// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"

void AShooterAIController::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr){
        RunBehaviorTree(AIBehavior);   //BT 실행
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 
        GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
        GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation());   //몬스터 첫 위치 블랙보드에 저장 
        //블랙보드에 있는 PlayerLocation 변수에 플레이어의 위치를 저장
    }
}
void AShooterAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    //해당 코드는 BT_PlayerLocationIfSeen.cpp에 대체함. 
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 


    // if(LineOfSightTo(PlayerPawn)){
    //     //플레이어 위치 설정
    //     //플레이어 마지막 위치도 설정 
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), PlayerPawn->GetActorLocation()); //플레이어 위치 블랙보드에 저장
    // }
    // else{
    //     //플레이어 위치 clear 
    //     GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
    // }



    //아래 코드는 BT로 바꿀 수 있음! 
    // APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); //몬스터가 캐릭터 방향 보게끔 

    // //시선이 있으면 MoveTo, SetFocus
    // //시선이 없으면 ClearFocus, StopMovement 
    // if(LineOfSightTo(PlayerPawn)){
    //     SetFocus(PlayerPawn);
                
    //     MoveToActor(PlayerPawn);    //플레이어한테 돌진!!! 

    //     // MoveToActor(PlayerPawn, AcceptanceRadius);    //플레이어한테 돌진!!! 
    // }

    // else{
    //     ClearFocus(EAIFocusPriority::Gameplay);   //포커스 지우기(플레이어 추적 중단)
    //     StopMovement();
    // }
}
