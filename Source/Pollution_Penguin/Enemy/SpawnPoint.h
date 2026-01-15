// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include"../Public/Enemy/WaveInfo.h"
#include"RoundInfo.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnPoint.generated.h"




UCLASS()
class POLLUTION_PENGUIN_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint();

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void NextWaveNotify();
	UFUNCTION(BlueprintImplementableEvent)
	void GameEnd_Blueprint();

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	UStaticMeshComponent* mStaticMesh;

	UPROPERTY(EditAnywhere, Category = "Spawning")
	TSubclassOf<class AActor> ActorToSpawn;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TArray<TSubclassOf<class AActor>> myEnemy;

	UPROPERTY(EditAnywhere, Category = "Spawn Points")
	TArray<AActor*> TargetPoints;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FRoundInfoStruct> RoundInfos;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	double MonsterSpawnDelay=0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Replicated)
	int nowRound = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Replicated)
	int nowWave = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	bool areAllEnemyDead = true;


	UFUNCTION(BlueprintCallable)
	void SpawnWave_Direct(int round, int wave);


	UFUNCTION(BlueprintCallable)
	void SetRoundAndWave(int round, int wave);


	UPROPERTY(BlueprintReadOnly)
	double lastRoundTime;
private:
	UFUNCTION(BlueprintCallable)
	void SpawnEnemy(UClass* spawnMonster, FVector SpawnLocation, FRotator spawnRotation);
	double NowSpawnTerm();
	UFUNCTION(BlueprintCallable)
	bool GoToNextWave();

	
	float currentTime;
	double nextSpawnTerm=3.0f;
	double lastSpawnTime;


};
