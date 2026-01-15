// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Sweep.h"
#include "AIcontroller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Map/GlacierPlatform.h"
#include "DrawDebugHelpers.h"


UBTService_Sweep::UBTService_Sweep()
{
     NodeName = "Is Hit? (Sweep)";
}

void UBTService_Sweep::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    Enemy = OwnerComp.GetAIOwner()->GetPawn();

    // 돌진 방향 설정
    FRotator EnemyRotation = Enemy->GetActorRotation();

    TArray<FHitResult> HitResults;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(Enemy); // 자기 자신은 무시


    FCollisionShape CollisionShape;
    FVector3f BoxExtent(50.0f, 50.0f, 100.0f);  //BoxExtend: sweep 방식 사용용 박스 생성 
    CollisionShape.SetBox(BoxExtent);  

    FVector StartBox = Enemy->GetActorLocation() + FRotationMatrix(EnemyRotation).GetUnitAxis(EAxis::X) * 200.0f;
    FVector EndBox = StartBox + FRotationMatrix(EnemyRotation).GetUnitAxis(EAxis::Z) * -200.0f;

    //DrawDebugBox(GetWorld(), StartBox + (EndBox - StartBox) * 0.5f, FVector(BoxExtent), FQuat::Identity, FColor::Red, false, 1.0f);

    //DrawDebugBoxes(StartBox, EndBox, BoxExtent, CollisionShape);

    //충돌 검사 
    bool bHit = Enemy->GetWorld()->SweepMultiByChannel(HitResults, StartBox, EndBox, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams);

    UpdateBlackboard(OwnerComp, bHit, HitResults);

}

//충돌 검사하는 부분 
void UBTService_Sweep::UpdateBlackboard(UBehaviorTreeComponent& OwnerComp, bool bHit, const TArray<FHitResult>& HitResults)
{
    bool bFoundGlacierPlatform = false;

    // 돌진 방향 검사
    if (bHit)    {
        for (const FHitResult& HitResult : HitResults) {    //충돌한 리스트 중 빙하가 있는지 확인
            AActor* HitActor = HitResult.GetActor();
            if (Cast<AGlacierPlatform>(HitActor)) {
                bFoundGlacierPlatform = true;
                break;
            }
        }
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), bFoundGlacierPlatform); //빙하없으면 ㅈㅈ 
    }
    else if(!bHit)
    {
        //바닥이 없는 경우 
        OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), false);  //히트자체가 없으면 ㅈㅈ
    }

}

void UBTService_Sweep::DrawDebugBoxes(const FVector& StartBox, const FVector& EndBox, FVector3f BoxExtent, const FCollisionShape& CollisionShape) const {
    const int32 NumLayers = 5; // 박스 몇겹?
    for (int32 i = 0; i < NumLayers; ++i)
    {
        FVector Offset = FVector(0.0f, 0.0f, i * 2.0f); // 겹마다 층수 조금씩 높임 
        DrawDebugBox(GetWorld(), StartBox + (EndBox - StartBox) * 0.5f + Offset, FVector(BoxExtent), FQuat::Identity, FColor::Red, false, 1.0f);
    }

}