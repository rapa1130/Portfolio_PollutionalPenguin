// Fill out your copyright notice in the Description page of Project Settings.


#include "Push_Enemy.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
APush_Enemy::APush_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    

}


// Called when the game starts or when spawned
void APush_Enemy::BeginPlay()
{
    Health = MaxHealth;
	Super::BeginPlay();
	
}

// Called every frame
void APush_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float APush_Enemy::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	//루트꺼 가져오기 
	DamageToApply = FMath::Min(Health, DamageToApply); //남은 Health값보다 데미지 수치가 높은경우 대비.. 
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("몬스터 남은 체력: %f"), Health); 

	// if (IsDead()){
	// 	DetachFromControllerPendingDestroy();	//컨트롤러와의 연결 끊기
	// 	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);	//콜리전 비활성화
	// }

	return DamageToApply;
}

void APush_Enemy::Push()
{


// USkeletalMeshComponent* MeshComponent = GetMesh();
// if (MeshComponent && MeshComponent->SkeletalMesh)
// {
//     UPhysicsAsset* PhysicsAsset = MeshComponent->GetPhysicsAsset();
//     if (PhysicsAsset)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Physics Asset Found"));
        
//     }
// }

	// 돌진 방향 설정
    FVector PushDirection = GetActorForwardVector();

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this); // 자기 자신은 무시


    FCollisionShape CollisionShape;
    FVector3f BoxExtent(100.0f, 100.0f, 100.0f);  //BoxExtend: sweep 방식 사용용 박스 생성 
    CollisionShape.SetBox(BoxExtent);  

    FVector Start = GetActorLocation();
    FVector End = Start + PushDirection * 150.0f;

    //DrawDebugBox(GetWorld(), Start + (End - Start) * 0.5f, FVector(BoxExtent), FQuat::Identity, FColor::Red, false, 1.0f);


    const int32 NumLayers = 5; // 박스 몇겹?
    for (int32 i = 0; i < NumLayers; ++i)
    {
        FVector Offset = FVector(0.0f, 0.0f, i * 2.0f); // 겹마다 층수 조금씩 높임 
        DrawDebugBox(GetWorld(), Start + (End - Start) * 0.5f + Offset, FVector(BoxExtent), FQuat::Identity, FColor::Red, false, 1.0f);
    }

    // 돌진 방향 검사
    if (GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + PushDirection * 150.0f, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams))
    {
        //플레이어와 충돌했다면
        if (ACharacter* HitCharacter = Cast<ACharacter>(HitResult.GetActor()))
        {
            // 밀치기
            FVector LaunchVelocity = HitCharacter->GetVelocity() + PushDirection * 1000.0f;
            LaunchVelocity.Z += 500.0f;
            HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
            UE_LOG(LogTemp, Warning, TEXT("Push"));
        }
    }

    //자신의 위치 업데이트
    // SetActorLocation(GetActorLocation() + PushDirection * 50.0);


	// ACharacter* Player = Cast<ACharacter>(UGameplayStatics::GetPlayerController(this, 0)->GetCharacter());


	// if(Player){
	// 	FVector PushDirection = Player->GetActorLocation() - GetActorLocation();
	// 	PushDirection.Normalize();	

	// 	Player->LaunchCharacter(PushDirection * 1000, true, true);
	// }
}

// void APush_Enemy::PushMove(FVector Direction, int Speed)
// {
//     UE_LOG(LogTemp, Warning, TEXT("PushMove 실행! "));
//     FVector Start = this->GetActorLocation() + FVector(100, 0, 0);
//     FVector End = this->GetActorLocation() + FVector(100, 0, -500);

//     FHitResult HitResult;
//     FCollisionQueryParams CollisionParams;
//     CollisionParams.AddIgnoredActor(this);

//     bool bHit = this->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

//     FColor LineColor = bHit ? FColor::Red : FColor::Green;
//     float LineThickness = 100.0f;
//     DrawDebugLine(this->GetWorld(), Start, End, LineColor, false, -1, 0, LineThickness);

//     if (bHit)
//     {
//         UE_LOG(LogTemp, Warning, TEXT("Hit!"));
//         //FVector NewLocation = this->GetActorLocation() + (Speed * 1000 * Direction * GetWorld()->GetDeltaSeconds());
//         //this->SetActorLocation(NewLocation, true);
//         //UE_LOG(LogTemp, Warning, TEXT("New Location: %s"), *NewLocation.ToString());
//     }
//     else
//     {
//         UE_LOG(LogTemp, Warning, TEXT("not Hit!"));
//         return;
//     }
// }

// Called to bind functionality to input
void APush_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
