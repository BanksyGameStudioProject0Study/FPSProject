// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (!CollisionComponent) {
		//���ü򵥵�������ײ
		CollisionComponent = CreateAbstractDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//������ײ���
		CollisionComponent->InitSphereRadius(15.0f);
		//�����������Ϊ��ײ���
		RootComponent = CollisionComponent;	
	}
	//��ʼ���ӵ�ʵ�������������ʼ���صģ�����ٶȣ���ת�������
	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed=3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	ProjectileMeshComponent = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	ProjectileMeshComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSProjectile::FireInDirection(const FVector& ShootDirection) {

	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Now is using FPSCharater"));
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}