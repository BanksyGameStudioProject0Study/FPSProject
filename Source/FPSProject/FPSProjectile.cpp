// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSProjectile.h"

// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (!CollisionComponent) {
		//设置简单的球体碰撞
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		//撒和值碰撞通道
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		//设置碰撞体积
		CollisionComponent->InitSphereRadius(15.0f);
		//将根组件设置为碰撞组件
		RootComponent = CollisionComponent;	
	}
	//初始化子弹实例（绑定组件，初始化素的，最大速度，旋转体变量）
	if (!ProjectileMovementComponent) {
		ProjectileMovementComponent = CreateAbstractDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		//更新Actor位置
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		//设置初始速度与最大速度
		ProjectileMovementComponent->InitialSpeed=3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
	ProjectileMeshComponent = CreateAbstractDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
	//获取资源，绑定网格体
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Sphere.Sphere'"));
	ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
	//设置缩放
	ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
	//设置附加与根组件
	ProjectileMeshComponent->SetupAttachment(RootComponent);
	//生命周期
	InitialLifeSpan = 3.0f;
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

//开火函数
void AFPSProjectile::FireInDirection(const FVector& ShootDirection) {

	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Now is using Fire"));
	//设置移动组件向量值
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics()) {
		OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	Destroy();
}