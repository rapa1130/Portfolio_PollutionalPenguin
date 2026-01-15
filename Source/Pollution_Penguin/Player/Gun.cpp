// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ShooterCharacter.h"
#include "Config/InGamePlayerController.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    //메쉬 부착 관련 파트 
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

    shooterCharacter = Cast<AShooterCharacter>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void AGun::PullTrigger_Implementation()
{
    /*if (MuzzleFlash)
    {
        UNiagaraFunctionLibrary::SpawnSystemAttached(
            MuzzleFlash,                            // 나이아가라 시스템 애셋
            Mesh,                                   // 부착할 메쉬
            TEXT("BarrelEndSocket"),                // 소켓 이름
            FVector::ZeroVector,                    // 위치 오프셋
            FRotator::ZeroRotator,                  // 회전 오프셋
            EAttachLocation::SnapToTargetIncludingScale,  // 부착 위치 옵션
            true                                    // 자동 파괴 여부
        );
    }*/

    //UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("BarrelEndSocket"));
    SpawnBullet_Implementation();
    Test();
}

void AGun::SpawnBullet_Implementation() {
    //총알 위치, 각도 설정 
    APawn* OwnerPawn = Cast<APawn>(GetOwner()); //컨트롤 가져오려면 폰만 가능함. 
    if (OwnerPawn == nullptr) return;

    FVector Location = OwnerPawn->GetActorLocation();
    FRotator Rotation = OwnerPawn->GetActorRotation();
    //FTransform BarrelEndSocket = Mesh->GetSocketTransform(TEXT("BarrelEndSocket"));

    //총알 실제 생성, 발사!  
    ABullet* bullet = GetWorld()->SpawnActor<ABullet>(bulletFactory, Location, Rotation);
    if (bullet == nullptr) return;
    bullet->SetOwner(OwnerPawn);

}

void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

