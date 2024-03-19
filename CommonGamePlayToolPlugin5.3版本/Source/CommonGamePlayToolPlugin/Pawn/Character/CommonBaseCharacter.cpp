#include "CommonBaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#pragma region 内部使用函数
ACommonBaseCharacter::ACommonBaseCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    SpringArmComponent=CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
    SpringArmComponent->SetupAttachment(RootComponent);
	//SpringArmComponent->TargetArmLength=0.f;
	
    CameraComponent=CreateDefaultSubobject<UCameraComponent>(TEXT("FirstViewCamera"));
    CameraComponent->SetupAttachment(SpringArmComponent);
	
}

void ACommonBaseCharacter::ReceiveInputRotateEvent_Implementation(const FVector2D& Value)
{
	//计算X轴的输入
	const float NewYawValue=SpringArmComponent->GetComponentRotation().Yaw+Value.X*RotateSpeed.X*GetWorld()->DeltaTimeSeconds;
	//计算Y轴的输入
	const float NewPitchValue=UKismetMathLibrary::FClamp((SpringArmComponent->GetComponentRotation().Pitch+Value.Y*RotateSpeed.Y*GetWorld()->DeltaTimeSeconds),RotateRange.X,RotateRange.Y);
	
	//更新设置USpringArmComponent的角度
	SpringArmComponent->SetWorldRotation(FRotator(NewPitchValue,NewYawValue,SpringArmComponent->GetComponentRotation().Roll));
	
}

void ACommonBaseCharacter::ReceiveInputMoveEvent_Implementation(const FVector2D& Value)
{
	//先计算X轴的移动
	if (Value.X!=0.f)
	{
		//获取视角X轴右侧的向量
		const FVector XAxisRightVector=UKismetMathLibrary::GetRightVector(GetCurrentCameraRotate());

		//最终X轴移动的向量
		const FVector XAxisMoveVector=XAxisRightVector*Value.X*MoveSpeed.X;

		//添加X轴的移动向量
		GetCharacterMovement()->AddInputVector(XAxisMoveVector);
	}
	
	//先计算X轴的移动
	if (Value.Y!=0.f)
	{
		//获取视角Y轴前方的向量
		const FVector YAxisForwardVector=UKismetMathLibrary::GetForwardVector(GetCurrentCameraRotate());

		//最终Y轴移动的向量
		const FVector YAxisMoveVector=YAxisForwardVector*Value.Y*MoveSpeed.Y;

		//添加X轴的移动向量
		GetCharacterMovement()->AddInputVector(YAxisMoveVector);
	}
	
}

FRotator ACommonBaseCharacter::GetCurrentCameraRotate() const
{
	if (Controller)
	{
		if (APlayerController const * PC=Cast<APlayerController>(Controller))
		{
			return PC->PlayerCameraManager->GetCameraRotation();
		}
	}
	return FRotator();
}
#pragma endregion