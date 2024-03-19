// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonDefaultPawn.h"
#include "../../Component/Input/InputBaseComponent.h"
#include "../../CommonGamePlayToolPlugin.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

//MovementComponentClass

// Sets default values
ACommonDefaultPawn::ACommonDefaultPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//创建对应的碰撞反馈组件
	if (CollisionShapeComponentType==ECollisionShapeComponentType::Box)
	{
		UBoxComponent* BoxComponent=CreateDefaultSubobject<UBoxComponent>(TEXT("UBoxComponent"));
		CollisionComponent=BoxComponent;
		
	}else if (CollisionShapeComponentType==ECollisionShapeComponentType::Capsule)
	{
		UCapsuleComponent* CapsuleComponent=CreateDefaultSubobject<UCapsuleComponent>(TEXT("UCapsuleComponent"));
		CollisionComponent=CapsuleComponent;
		CapsuleComponent->InitCapsuleSize(CapsuleRadius,CapsuleHeight);
	}
	else
	{
		USphereComponent* SphereComponent=CreateDefaultSubobject<USphereComponent>(TEXT("USphereComponent"));
		CollisionComponent=SphereComponent;
		SphereComponent->InitSphereRadius(SphereRadius);
	}
	
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	
	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("PawnMovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;

	//将弹簧臂组件挂载在根组件上
	SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength=0.0f;
	
	//将相机挂载在弹簧臂上
	CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

// Called when the game starts or when spawned
void ACommonDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACommonDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACommonDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACommonDefaultPawn::RegisterInputMappding_Implementation()
{
	//遍历查找当前Pawn上的UInputBaseComponent,再调用一次注册,防止调用OnPossess之后Pawn的输入映射清空的问题
	TArray<UInputBaseComponent*> InputBaseComponentArray;
	this->GetComponents(InputBaseComponentArray);
	for (UInputBaseComponent* InputBaseComponent:InputBaseComponentArray)
	{
		if (InputBaseComponent->bAuto_Register_Input_Mapping) InputBaseComponent->RegisterInputMapping();
	}
}

void ACommonDefaultPawn::ReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value)
{
	if (Value.X != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlRotation = Controller->GetControlRotation();
			AddMovementInput( FRotationMatrix(FRotationMatrix(FRotator(
				bCanMoveInSky?ControlRotation.Pitch:(ControlRotation.Pitch>0?0:ControlRotation.Pitch),
				ControlRotation.Yaw,
				ControlRotation.Roll))).GetScaledAxis( EAxis::X ), Value.X*PawnMoveSpeed.X );
		}
	}
	
	if (Value.Y != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlRotation = Controller->GetControlRotation();
			AddMovementInput( FRotationMatrix(FRotator(
				bCanMoveInSky?ControlRotation.Pitch:(ControlRotation.Pitch>0?0:ControlRotation.Pitch),
				ControlRotation.Yaw,
				ControlRotation.Roll)).GetScaledAxis( EAxis::Y ), Value.Y*PawnMoveSpeed.Y);
		}
	}
}

void ACommonDefaultPawn::ReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value)
{
	AddControllerYawInput(Value.X * ViewTurnSpeed.X * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	AddControllerPitchInput(Value.Y * ViewTurnSpeed.Y * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}
#pragma region 视角相关参数设置
void ACommonDefaultPawn::SetCameraFOV(const float CameraFOV)
{
	CameraComponent->SetFieldOfView(CameraFOV);
}


#pragma endregion
void ACommonDefaultPawn::ReceiveFloatInputUpEvent_Implementation(const float& Value)
{
	if (Value != 0.f)
    {
    	AddMovementInput(FVector::UpVector, Value);
    }
}
