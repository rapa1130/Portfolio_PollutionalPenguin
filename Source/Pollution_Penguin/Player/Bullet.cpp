// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include "Engine/World.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "../Config/InGamePlayerController.h"
#include "Kismet/GameplayStatics.h"



ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체 인스턴스 등록 
	collisionComp = CreateDefaultSubobject <USphereComponent> (TEXT("Enemy"));
	collisionComp->SetCollisionProfileName(TEXT("BlockAll"));
	collisionComp->SetSphereRadius(13);	//충돌체 반경 
	RootComponent = collisionComp;

	//스태틱 메쉬 부착
	bodyMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComp"));
	bodyMeshComp->SetupAttachment(collisionComp);
	bodyMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//충돌 ㄴㄴ 
	bodyMeshComp->SetRelativeScale3D(FVector(5.0f));	//스태틱 메쉬 크기 
	bodyMeshComp->SetRelativeLocation(FVector(0, 0, -12.5f));	//충돌 범위로부터 얼마나 위치 조정? 

	auto sphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Assets/LPG2/LPG2Meshes/Bullets/OBJ_SMGRound_01.OBJ_SMGRound_01'"));
	if (sphereMesh.Succeeded())
	{
		bodyMeshComp->SetStaticMesh(sphereMesh.Object);
	}

	//총알 속도, 탄성 
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComp"));
	movementComp->SetUpdatedComponent(collisionComp);
	movementComp->InitialSpeed = 2000;
	movementComp->MaxSpeed = 2000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;

	InitialLifeSpan = 10.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle deathTimer;
	GetWorld()->GetTimerManager().SetTimer(deathTimer, this, &ABullet::Die, 2.0f, false);

	GlacierPlatformArrangement = Cast<AGlacierPlatformArrangement>(UGameplayStatics::GetActorOfClass(GetWorld(), AGlacierPlatformArrangement::StaticClass()));

	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnSphereComponentBeginOverlap);

	
}

void ABullet::Die()	//총알 삭제 
{
	Destroy();
}


void ABullet::OnSphereComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(bHasHitEnemy) return;

	if (OtherActor->ActorHasTag("Player")) return;
	
	//UE_LOG(LogTemp, Warning, TEXT("총알이 닿은 오브젝트: %s"), *OtherActor->GetName());

	if (OtherActor->ActorHasTag("Enemy") || OtherActor->ActorHasTag("GlacierPlatform")){	
		Destroy();	//총알 삭제 

		bHasHitEnemy = true; 
	
		FVector Location = this->GetActorLocation();
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				AInGamePlayerController* pc = Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
				if (pc) {
					pc->CreateGlacierPlatformsHexagonShpae(2, Location, { 40,20});
				}
			}
		}

		
	}
}


// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->GetActorLocation().Z < -10)
	{
		FVector Location = this->GetActorLocation();
		if (GEngine)
		{
			UWorld* World = GEngine->GetWorldFromContextObjectChecked(this);
			if (World)
			{
				AInGamePlayerController* pc=Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(World, 0));
				if (pc) {
					pc->CreateGlacierPlatformsHexagonShpae(2, Location, {40, 20});
				}
			}
		}
		Destroy();

	}

}

