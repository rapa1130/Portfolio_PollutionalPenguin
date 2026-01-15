// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_LineTracing.h"
#include "Map/GlacierPlatform.h"
#include "DrawDebugHelpers.h"
#include "AIcontroller.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTService_LineTracing::UBTService_LineTracing()
{
    NodeName = "Is Hit? (Line Tracing)";
}


void UBTService_LineTracing::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
    Enemy = OwnerComp.GetAIOwner()->GetPawn();

    //몬스터 회전해도 RAY 따라가게 설정
    FRotator EnemyRotation = Enemy->GetActorRotation(); 

    FVector StartRay = GetStartRay(EnemyRotation);
    FVector EndRay = StartRay + FRotationMatrix(EnemyRotation).GetUnitAxis(EAxis::Z) * -500.0f;

    TArray<FHitResult> HitResults;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(Enemy);

    bool bHit = Enemy->GetWorld()->LineTraceMultiByChannel(HitResults, StartRay, EndRay, ECC_Visibility, CollisionParams);

    //!!!!! 디버깅용, 나중에 삭제하기 !!!!!!
    FColor LineColor = bHit ? FColor::Red : FColor::Green;
    float LineThickness = 100.0f;
    DrawDebugLine(Enemy->GetWorld(), StartRay, EndRay, LineColor, false, -1, 0, LineThickness);

    // 돌진 방향 검사
    if (bHit)    {
        HandleHit(OwnerComp, HitResults);
    }
}

void UBTService_LineTracing::HandleHit(UBehaviorTreeComponent& OwnerComp, const TArray<FHitResult>& HitResults)
{
    bool bFoundGlacierPlatform = false;

    //육지를 만났다면 
    for (const FHitResult& HitResult : HitResults) {
        if (Cast<AGlacierPlatform>(HitResult.GetActor())) {
            bFoundGlacierPlatform = true;
            break; // AGlacierPlatform을 찾았으므로 반복 종료
        }
    }

    if (bFoundGlacierPlatform) {
        FVector Location = Enemy->GetActorLocation();

        if (Location.Z < 40) {
            Location.Z += 50;
            Enemy->SetActorLocation(Location);
        }

        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);
        //UE_LOG(LogTemp, Warning, TEXT("육지다~~"));
    }
}

FVector UBTService_LineTracing::GetStartRay(const FRotator& EnemyRotation) const
{
    return Enemy->GetActorLocation() +
           FRotationMatrix(EnemyRotation).GetUnitAxis(EAxis::X) * 100.0f +
           FRotationMatrix(EnemyRotation).GetUnitAxis(EAxis::Z) * 300.0f;
}


