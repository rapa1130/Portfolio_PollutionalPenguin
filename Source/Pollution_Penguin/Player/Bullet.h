// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Map/GlacierPlatformArrangement.h"


#include "Bullet.generated.h"


UCLASS()
class POLLUTION_PENGUIN_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Die();

	// UFUNCTION()
	// void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category=Movement)
	class UProjectileMovementComponent * movementComp;

	//충돌 감지
	UPROPERTY(VisibleAnywhere, Category = Collision)
	class USphereComponent * collisionComp;

	//총알 외관
	UPROPERTY(EditAnywhere, Category=Mesh)
	class UStaticMeshComponent * bodyMeshComp;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFlash;

	UPROPERTY()
	UBoxComponent* Box;

	UPROPERTY(EditAnywhere)
	USoundBase * ImpactEfeect;	//몬스터 맞췄을 때 나는 소리 

private:
	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY(EditAnywhere)
	AGlacierPlatformArrangement * GlacierPlatformArrangement;

	bool bHasHitEnemy = false; 
};
