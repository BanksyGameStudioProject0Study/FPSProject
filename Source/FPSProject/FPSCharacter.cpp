// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FPSCameraComponent = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	check(FPSCameraComponent != nullptr);
	FPSCameraComponent->SetupAttachment(CastChecked<USceneComponent,UCapsuleComponent>(GetCapsuleComponent()));
	FPSCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f,50.0f+BaseEyeHeight));
	//使用Pawn控制摄像机
	FPSCameraComponent->bUsePawnControlRotation = true;
	FPSMesh = CreateAbstractDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	check(FPSMesh != nullptr);
	//设置第一人称视角仅拥有者可见
	FPSMesh->SetOnlyOwnerSee(true);
	FPSMesh->SetupAttachment(FPSCameraComponent);
	//取消阴影
	FPSMesh->bCastDynamicShadow = false;
	FPSMesh->CastShadow = false;
	//设置第三人称不可见
	GetMesh()->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	check(GEngine != nullptr);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Now is using FPSCharater"));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// 设置"移动"绑定。
	PlayerInputComponent->BindAxis("MoveForward",this, &AFPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	//设置相机绑定
	PlayerInputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);
	//设置跳跃绑定
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::StopJump);
	//设置开火绑定
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::Fire);
}
void AFPSCharacter::MoveForward(float Value)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}//向前移动函数
void AFPSCharacter::MoveRight(float Value) {
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}//向右移动函数
void AFPSCharacter::StartJump() {
	bPressedJump = true;
}
void AFPSCharacter::StopJump() {
	bPressedJump = false;
}
void AFPSCharacter::Fire() {
	if (ProjectileClass) {
		FVector CameraLocation;
		FRotator CameraRotation;
		//获取视角位置和旋转体
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);
		//设置发射的位置和旋转体
		FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		FRotator MuzzleRotation = CameraRotation;
		MuzzleRotation.Pitch += 10.0f;
		//将位置和旋转体转换为世界场景
		UWorld* World = GetWorld();
		if (World) {
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			AFPSProjectile* Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile) {
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
			}
		}
	}
}