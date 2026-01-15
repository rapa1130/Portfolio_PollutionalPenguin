// Fill out your copyright notice in the Description page of Project Settings.


#include "Push_Enemy2.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIcontroller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
APush_Enemy2::APush_Enemy2()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
}

// Called when the game starts or when spawned
void APush_Enemy2::BeginPlay()
{
	Health = MaxHealth;
	Super::BeginPlay();

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
void APush_Enemy2::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 1, 1), GetActorRotation(), true);
	GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, -1, -1), GetActorRotation(), true);

	
}


float APush_Enemy2::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, 
                               class AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	//루트꺼 가져오기 
	DamageToApply = FMath::Min(Health, DamageToApply); //남은 Health값보다 데미지 수치가 높은경우 대비.. 
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("몬스터 남은 체력: %f"), Health); 
	SpawnEffect(DamageCauser->GetActorLocation());
	
	/*UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),                                  
			MuzzleFlash,                                 
			GetActorLocation(),                                  
			GetActorRotation(),                                  
			FVector(5.0f),                                // 스케일
			true                                          // 자동 파괴 여부
		);


	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEfeect, GetActorLocation());
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
void APush_Enemy2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APush_Enemy2::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APush_Enemy2, Health);
}