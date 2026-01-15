// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "PushEnemy2_Controller.generated.h"

/**
 * 
 */
UCLASS()
class POLLUTION_PENGUIN_API APushEnemy2_Controller : public AAIController
{
	GENERATED_BODY()
	

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior; 
};
