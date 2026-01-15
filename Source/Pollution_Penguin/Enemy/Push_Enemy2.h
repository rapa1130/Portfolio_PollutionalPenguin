// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Push_Enemy2.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API APush_Enemy2 : public ACharacter
{
	GENERATED_BODY()

public:
	APush_Enemy2();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnEffect(FVector location);

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(VisibleAnywhere, Replicated)
	float Health;

public:
	UPROPERTY(EditAnywhere, Category = "Option", meta = (AllowPrivateAccess = "true"));
	int32 Speed;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere)
	USoundBase * ImpactEfeect;	//몬스터 맞췄을 때 나는 소리 

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DieFlash;

};
