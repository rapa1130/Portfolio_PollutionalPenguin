// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraFunctionLibrary.h"
#include "Bullet.h"
#include "../Player/ShooterCharacter.h"
#include "Gun.generated.h" 


UCLASS()
class POLLUTION_PENGUIN_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	UFUNCTION(Server,Reliable)
	void PullTrigger();

	UFUNCTION(BlueprintImplementableEvent)
	void Test();

	UFUNCTION(Server, Reliable)
	void SpawnBullet();


	// UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("BarrelEndSocket"));
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite);
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double ShootTerm=1.0;



private:
	UPROPERTY(EditAnywhere)
	AShooterCharacter* shooterCharacter;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
	
	

	UPROPERTY(EditAnywhere);
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase * MuzzleSound;

	UPROPERTY(EditAnywhere);
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere);
	float MaxRange = 1000; 

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditDefaultsOnly,Category=BulletFactory)
	TSubclassOf<class ABullet> bulletFactory;


	
};
