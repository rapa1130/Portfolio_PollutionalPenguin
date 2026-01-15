// Fill out your copyright notice in the Description page of Project Settings.


#include "Squid.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"


ASquid::ASquid()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

void ASquid::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
	{
		return;
	}

	UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
	if (BlackboardComp == nullptr)
	{
		return;
	}
}

// Called every frame
void ASquid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 1, 1), GetActorRotation(), true);
	GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, -1, -1), GetActorRotation(), true);

}


float ASquid::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	//루트꺼 가져오기 
	DamageToApply = FMath::Min(Health, DamageToApply); //남은 Health값보다 데미지 수치가 높은경우 대비.. 
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("몬스터 남은 체력: %f"), Health); 

	DamageEffect();

	/*
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),                                  
			MuzzleFlash,                                 
			GetActorLocation(),                                  
			GetActorRotation(),                                  
			FVector(5.0f),                                // 스케일
			true                                          // 자동 파괴 여부
		);

	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEfeect, GetActorLocation());
	*/

	if(Health <= 0)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),                                  
			DieFlash,                                 
			GetActorLocation(),                                  
			GetActorRotation(),                                  
			FVector(20.0f),                                
			true                                          
		);
		Destroy();
	}


	return DamageToApply;
}


// Called to bind functionality to input
void ASquid::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
