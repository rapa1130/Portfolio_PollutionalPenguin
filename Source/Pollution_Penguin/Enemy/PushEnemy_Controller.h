// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PushEnemy_Controller.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API APushEnemy_Controller : public AAIController
{
	GENERATED_BODY()
	
public:
	void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; 

};
