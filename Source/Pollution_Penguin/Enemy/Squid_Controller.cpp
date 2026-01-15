// Fill out your copyright notice in the Description page of Project Settings.


#include "Squid_Controller.h"


void ASquid_Controller::BeginPlay()
{
    Super::BeginPlay();

    if(AIBehavior != nullptr){
        RunBehaviorTree(AIBehavior);   //BT 실행
    }
}