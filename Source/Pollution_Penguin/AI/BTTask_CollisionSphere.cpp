// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_CollisionSphere.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "AiController.h"
#include "AIUtility.h"
#include "Kismet/GameplayStatics.h"


UBTTask_CollisionSphere::UBTTask_CollisionSphere()
{
    NodeName = "CollisionSphere";
    bCreateNodeInstance = true; //인스턴스마다 노드 생성하도록 함! 

    InitializeCollisionSphere();
}

//충돌 설정 
void UBTTask_CollisionSphere::InitializeCollisionSphere()
{
    CollisionSphere = nullptr;

    CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
    CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 활성화
    CollisionSphere->SetCollisionObjectType(ECollisionChannel::ECC_OverlapAll_Deprecated); // 충돌 객체 유형 설정
    CollisionSphere->SetCollisionResponseToAllChannels(ECR_Ignore); // 모든 채널 무시
    CollisionSphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap); // 오버랩하도록 설정

    CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &UBTTask_CollisionSphere::OnOverlapBegin);

}




EBTNodeResult::Type UBTTask_CollisionSphere::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ResetGlacierList(); //리스트 초기화

    APawn * OwnerActor = OwnerComp.GetAIOwner()->GetPawn();
    float GlacierSize = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Size"));

    SetupCollisionSphere(OwnerActor, GlacierSize);  //위치 설정 

    if(GlacierSize >= 10000 && GlacierList.Num() == 0){
        UGameplayStatics::GetAllActorsOfClass(OwnerComp.GetWorld(), AGlacierPlatform::StaticClass(), GlacierList);
        if (GlacierList.Num() == 0){
            return EBTNodeResult::Failed;
        }
    }
    
    // DrawDebugSphere(
    //         GetWorld(),
    //         CollisionSphere->GetComponentLocation(),
    //         GlacierSize,
    //         12, 
    //         FColor::Red, // 색상
    //         false, // 지속적으로 그릴지 여부
    //         1.0f // 지속 시간
    //     );
    
    ClosestGlacier = FindClosestTarget(GlacierList, OwnerActor->GetActorLocation()); //가장 가까운 빙하 찾기

    if (ClosestGlacier == nullptr){
        return EBTNodeResult::Failed;
    }

    OwnerComp.GetBlackboardComponent()->SetValueAsObject(TEXT("TargetGlacier"), ClosestGlacier);    //블랙보드값 업데이트
    
    CleanupCollisionSphere();   //리소스 정리용(콜리전 초기화)

    return EBTNodeResult::Succeeded;
}



//리스트 초기화
void UBTTask_CollisionSphere::ResetGlacierList()
{
    GlacierList.Empty();
    ClosestGlacier = nullptr;
}

//콜리전 부착 설정 
void UBTTask_CollisionSphere::SetupCollisionSphere(APawn* OwnerActor, float GlacierSize)
{
    CollisionSphere->SetRelativeLocation(FVector::ZeroVector); 

    if (!CollisionSphere->IsRegistered())
    {
        CollisionSphere->SetupAttachment(OwnerActor->GetRootComponent());
        CollisionSphere->RegisterComponent();
    }
    
    CollisionSphere->SetSphereRadius(GlacierSize);
    CollisionSphere->SetRelativeLocation(FVector::ZeroVector); 
}

//리소스 과부하 방지를 위해 풀어주는 작업 진행 
void UBTTask_CollisionSphere::CleanupCollisionSphere()
{
    CollisionSphere->UnregisterComponent(); 
    CollisionSphere->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}



void UBTTask_CollisionSphere::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(AGlacierPlatform::StaticClass())) {
		AGlacierPlatform* platform = Cast<AGlacierPlatform>(OtherActor);
        GlacierList.Add(platform);
	}
}