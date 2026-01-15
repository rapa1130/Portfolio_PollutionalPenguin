// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"


//#include "Components/CapsuleComponent.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	
	//무기 장착 관련 
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);	//총 생성
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));	//총을 소켓에 붙임
	Gun->SetOwner(this);	//총의 소유자를 플레이어로 설정

	PlayerController = Cast<APlayerController>(GetController());
	lastShootTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
}


void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCharacterMovement()->MoveUpdatedComponent(FVector(1, 1, 1), GetActorRotation(), true);
	GetCharacterMovement()->MoveUpdatedComponent(FVector(-1, -1, -1), GetActorRotation(), true);
	//UpdateLookAtCursor();


	if (isDead){
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 30.0f); 
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            WaterParticle, 
            SpawnLocation, 
            FRotator::ZeroRotator, 
            FVector(20.0f),
            true, // 자동으로 파괴할지 여부
            true // 스폰된 상태로 시작
        );
	}
}

//플레이어 입력 바인딩 관련 함수 
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//플레이어 입력 바인딩(움직이기, 카메락 조작, 점프)
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShooterCharacter::MoveRight);



	// //카메라 위 아래로 보는 효과인데 주석처리 할지 말지 상의 필요 
 	// PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &APawn::AddControllerPitchInput);
 	PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &APawn::AddControllerYawInput);

	//플레이어 방향 회전 
	//PlayerInputComponent->BindAxis(TEXT("LookRight"), this, &AShooterCharacter::Rotate);

	if(isDead){
		return;
	}
	//PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ShotWithSound);	//총 발사
}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);	//루트꺼 가져오기 
	DamageToApply = FMath::Min(Health, DamageToApply); //남은 Health값보다 데미지 수치가 높은경우 대비.. 
	Health -= DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("남은 체력: %f"), Health); 

	return DamageToApply;
}

void AShooterCharacter::ShotWithSound()
{
	if (isDead) {
		return;
	}
	if (ShootDelay > UGameplayStatics::GetRealTimeSeconds(GetWorld()) - lastShootTime) {
		return;
	}
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactEfeect, GetActorLocation());
	lastShootTime = UGameplayStatics::GetRealTimeSeconds(GetWorld());
	Shoot();

}



void AShooterCharacter::MoveForward(float AxisValue)
{
	if (isDead) {
		return;
	}

	FVector ForwardDirection = FVector(1, 0, 0); 
	AddMovementInput(ForwardDirection * AxisValue);	//AxisValue 만큼 전진 

	if (AxisValue != 0.0f && !isDead)
    {
        FVector SpawnLocation = GetActorLocation() - FVector(0, 0, 50.0f); 
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            WalkParticle, 
            SpawnLocation, 
            FRotator::ZeroRotator, 
            FVector(5.0f),
            true, // 자동으로 파괴할지 여부
            true // 스폰된 상태로 시작
        );
    }
}

void AShooterCharacter::MoveRight(float AxisValue)
{
	if (isDead) {
		return;
	}
	FVector RightDirection = FVector(0, 1, 0); 
	AddMovementInput(RightDirection * AxisValue);		

	if (AxisValue != 0.0f && !isDead)
    {
        // 플레이어의 위치를 가져와서 바닥에 이펙트를 생성
        FVector SpawnLocation = GetActorLocation() - FVector(0, 0, 50.0f); // 바닥에 이펙트를 생성하기 위해 Z축을 조정
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            GetWorld(),
            WalkParticle, 
            SpawnLocation, 
            FRotator::ZeroRotator, 
            FVector(5.0f), 
            true, // 자동으로 파괴할지 여부
            true // 스폰된 상태로 시작
        );
    }
}

void AShooterCharacter::Rotate(float AxisValue)
{
	//SetActorRotation(GetActorRotation() + FRotator(0,AxisValue * 1000,0));
}


//플레이어가 마우스 포인터 바라보게 하는 함수 
void AShooterCharacter::UpdateLookAtCursor()
{
    if (!PlayerController) return;

	FVector2D CursorPosition;
    if (GEngine->GameViewport->GetMousePosition(CursorPosition))
    {
        FVector WorldPosition, WorldDirection;
        UGameplayStatics::DeprojectScreenToWorld(PlayerController, CursorPosition, WorldPosition, WorldDirection);

        FHitResult HitResult;
        FVector End = WorldPosition + (WorldDirection * 10000); 
        if (GetWorld()->LineTraceSingleByChannel(HitResult, WorldPosition, End, ECC_Visibility))
        {
            FVector LookAtDirection = (HitResult.Location - GetActorLocation()).GetSafeNormal();
            FRotator NewRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
			FRotator CurrentRotation = GetActorRotation();
			
			NewRotation.Pitch = CurrentRotation.Pitch;

			SetActorRotation(NewRotation);
            if (!HasAuthority()) {
                ClientUpdateLookAtCursor(NewRotation);
            }

			CurrentRotation = NewRotation;
        }
    }
	
}

void AShooterCharacter::ClientUpdateLookAtCursor_Implementation(FRotator NewRotation)
{
	SetActorRotation(NewRotation);
}


void AShooterCharacter::Shoot_Implementation()
{
	if (isDead) {
		return;
	}
	Gun->PullTrigger(); 
	UE_LOG(LogTemp, Warning, TEXT("Shoot_Implementation is called!"));
	isShooting = true;
}

// void AShooterCharacter::LookUp(float AxisValue)
// {
// 	AddControllerPitchInput(AxisValue);
// }

void AShooterCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AShooterCharacter, isShooting);
	DOREPLIFETIME(AShooterCharacter, isDead);
	DOREPLIFETIME(AShooterCharacter, deadPosition);
}