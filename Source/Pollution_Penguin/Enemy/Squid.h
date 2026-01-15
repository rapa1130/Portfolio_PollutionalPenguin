// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Squid.generated.h"

UCLASS()
class POLLUTION_PENGUIN_API ASquid : public ACharacter
{
	GENERATED_BODY()

public:
	ASquid();

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite)
	class USphereComponent* CollisionSphere;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void DamageEffect();


protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	float Speed = 10.0f;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* DieFlash;

private:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase * ImpactEfeect;	//몬스터 맞췄을 때 나는 소리 

	UPROPERTY(VisibleAnywhere)
	float Health;
	
};
