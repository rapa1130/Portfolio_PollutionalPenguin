// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SquidMove.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "AIUtility.h"
#include "PushEnemy2_Controller.h"


UBTTask_SquidMove::UBTTask_SquidMove()
{
    NodeName = "SquidMove";
    bCreateNodeInstance = true;
    bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_SquidMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Squid = Cast<ASquid>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Squid) return EBTNodeResult::Failed;

    AAIController* AIController = Cast<AAIController>(OwnerComp.GetOwner());
    BlackboardComp = AIController->GetBlackboardComponent();

    InitializeCollisionComponent();
    isMoving = true;

    return EBTNodeResult::InProgress;
}

//오징어에 부착된 직육면체의 충돌설정 
void UBTTask_SquidMove::InitializeCollisionComponent()
{
    CollisionComponent = Squid->FindComponentByClass<UBoxComponent>();

    if (CollisionComponent == nullptr)
    {
        return;
    }

    //기존에 등록된 델리게이트 삭제
    CollisionComponent->OnComponentHit.RemoveDynamic(this, &UBTTask_SquidMove::OnHit);
    CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &UBTTask_SquidMove::OnOverlap);
    CollisionComponent->OnComponentEndOverlap.RemoveDynamic(this, &UBTTask_SquidMove::OnEndOverlap);


    CollisionComponent->SetupAttachment(Squid->GetRootComponent()); 
    CollisionComponent->RegisterComponent(); 

    // 충돌 응답 설정
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
    
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); 


    CollisionComponent->OnComponentHit.AddDynamic(this, &UBTTask_SquidMove::OnHit);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UBTTask_SquidMove::OnOverlap);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &UBTTask_SquidMove::OnEndOverlap);
}




void UBTTask_SquidMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    if(Squid == nullptr){
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (!isMoving){
        if(Squid->Speed > 20.0f){
            MoveActorWithInertia(Squid, Direction, Squid->Speed, false, DeltaSeconds);
            return;
        }
        else{
            //메모리 누수 방지
            GetWorld()->GetTimerManager().ClearTimer(HitCooldownTimer);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }


    AActor * TargetGlacier = Cast<AActor> (OwnerComp.GetBlackboardComponent()->GetValueAsObject(TEXT("TargetGlacier")));
    AGlacierPlatform* GlacierPlatform = Cast<AGlacierPlatform>(TargetGlacier);


    //없앨 빙하가 월드에서 없어진 경우   
    if (GlacierPlatform == nullptr || GlacierPlatform->IsActorBeingDestroyed()) 
    {
        if(Squid->Speed > 20.0f){
            MoveActorWithInertia(Squid, Direction, Squid->Speed, false, DeltaSeconds);
            return;
        }
        else{
            OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("GlacierLocation"), GlacierLocation);
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
            return;
        }
    }   

    GlacierLocation = GlacierPlatform->GetActorLocation();

    //해당 방향으로 이동 
    MoveTarget(OwnerComp, TargetGlacier, DeltaSeconds);
    
    //해당 방향으로 회전 
    RotateToActor(OwnerComp, TargetGlacier, DeltaSeconds);


    //DrawDebugBox(GetWorld(), CollisionComponent->GetComponentLocation(), CollisionComponent->GetScaledBoxExtent(), CollisionComponent->GetComponentQuat(), FColor::Red, false, 0.5f);

    
}


void UBTTask_SquidMove::MoveTarget(UBehaviorTreeComponent& OwnerComp, AActor* TargetGlacier, float DeltaSeconds)
{
    FVector Location = Squid->GetActorLocation();
    FVector End = TargetGlacier->GetActorLocation();
    Direction = (End - Location).GetSafeNormal();
    Squid->Speed = 1000.0f;

    // if (Location.Z > 37.0f){
    //     FVector BounceDirection = - Squid->GetActorForwardVector();
    //     BounceDirection.Z = 0;
    //     Squid->LaunchCharacter(BounceDirection * 3000.0f, true, true);
    // }
    

    if (Direction.Size2D() < 0.1f)
    {
        //FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        HandleGlacierCollision(Cast<AGlacierPlatform>(TargetGlacier));
        return;
    }

    MoveActor(Squid, Direction, Squid->Speed, false, DeltaSeconds);
}


void UBTTask_SquidMove::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (bCanHit == false || !OtherActor->IsA(AGlacierPlatform::StaticClass())) {
        return;
    }

    AGlacierPlatform* Glacier = Cast<AGlacierPlatform>(OtherActor);
    HandleGlacierCollision(Glacier);
}

void UBTTask_SquidMove::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (Squid == nullptr || OtherActor == nullptr)
    {
        return;
    }

    if (OtherActor->IsA(AAIController::StaticClass()))
    {
        return;
    }

    // 이미 리스트에 있는지 확인
    if (!OverlappedActors.Contains(OtherActor))
    {
        OverlappedActors.Add(OtherActor); // 오버랩된 객체 추가
    }


    if (bCanHit == false || !OtherActor->IsA(AGlacierPlatform::StaticClass())) {
        return;
    }

    AGlacierPlatform* Glacier = Cast<AGlacierPlatform>(OtherActor);
    if (Glacier)
    {
        HandleGlacierCollision(Glacier);
    }
}

void UBTTask_SquidMove::OnEndOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor == nullptr)
    {
        return;
    }

    // 리스트에서 제거
    OverlappedActors.Remove(OtherActor);
}


void UBTTask_SquidMove::HandleGlacierCollision(AGlacierPlatform* Glacier)
{
    //오징어와 부딪친 빙하를 블랙보드에 업데이트
    BlackboardComp->SetValueAsObject(TEXT("TargetGlacier"), Glacier);
    GlacierLocation = Glacier->GetActorLocation();

    //바운스 하기
    FVector BounceDirection = (Squid->GetActorLocation() - Glacier->GetActorLocation()).GetSafeNormal();
    BounceDirection.Z = 0;
    Squid->LaunchCharacter(BounceDirection * 3000.0f, true, true);
    
    //빙하 체력 깎기
    Glacier->AdjustHealth(-30);

    //갉아먹는 소리
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), GnawEfeect, Glacier->GetActorLocation());


    //빙하 체력이 0이 되면서 Destroy되었다면 해당 BTTask 종료 
    if(Glacier == nullptr){
        isMoving = false;
    }
    
    bCanHit = false;
    GetWorld()->GetTimerManager().SetTimer(HitCooldownTimer, this, &UBTTask_SquidMove::ResetHitFlag, 0.001f, false);

}


void UBTTask_SquidMove::ResetHitFlag()
{
    bCanHit = true;
}
