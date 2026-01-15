// Fill out your copyright notice in the Description page of Project Settings.


#include "PushEnemy2_Controller.h"

void APushEnemy2_Controller::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr){
        RunBehaviorTree(AIBehavior);   //BT 실행
    }
}
