// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CommonBasePawn.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraActor.h"
#include "CommonGamePlayToolPlugin/Component/Input/InputBaseComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ACommonBasePawn::ACommonBasePawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//将Scene设置为根组件
	Scene=CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Scene);

	//弹簧臂组件挂到Scene上.
	SpringArm=CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Scene);

	//创建挂在弹簧臂上的相机,用于鸟瞰形式的移动.
	SpringArm_Camera=CreateDefaultSubobject<UCameraComponent>("SpringArm_Camera");
	SpringArm_Camera->SetupAttachment(SpringArm);
	//SpringArm_Camera->SetAutoActivate(true);
	
	//创建用于第一人称世界形式的移动的相机.
	Normal_Camera=CreateDefaultSubobject<UCameraComponent>("Normal_Camera");
	Normal_Camera->SetupAttachment(Scene);
	//Normal_Camera->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void ACommonBasePawn::BeginPlay()
{
	
	Super::BeginPlay();

	
	
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		UE_LOG(LogTemp,Error,TEXT("SpringArm_Camera和Normal_Camera都处于激活状态"));
	}
	
	//确保只有一个Camera处于激活状态
	Ensure_Only_One_Camera_Active_Internal();
}

// Called every frame
void ACommonBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TickTimerEvent(DeltaTime);
}

// Called to bind functionality to input
void ACommonBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//遍历查找当前Pawn上的UInputBaseComponent,再调用一次注册,防止调用OnPossess之后Pawn的输入映射清空的问题
	TArray<UInputBaseComponent*> InputBaseComponentArray;
	this->GetComponents(InputBaseComponentArray);
	for (UInputBaseComponent* InputBaseComponent:InputBaseComponentArray)
	{
		if (InputBaseComponent->bAuto_Register_Input_Mapping) InputBaseComponent->RegisterInputMapping();
	}
	
	
	SetupPlayerInputComponent_Internal();
}

void ACommonBasePawn::SetupPlayerInputComponent_Internal_Implementation()
{
}

int32 ACommonBasePawn::GetAllInput()
{
	UEnhancedInputComponent*  EnhancedInputComponent=(Cast<UEnhancedInputComponent>(this->InputComponent));
	auto AA= &EnhancedInputComponent->GetActionEventBindings();
	for (int i=0;i<AA->Num();i++)
	{
		
		GEngine->AddOnScreenDebugMessage(-1,0.f,FColor::White,EnhancedInputComponent->GetActionEventBindings()[i].Get()->GetAction()->GetName());
	}
	return  Cast<UEnhancedInputComponent>(this->InputComponent)->GetActionEventBindings().Num();
}

void ACommonBasePawn::TickTimerEvent_Implementation(float DeltaTime)
{
	Auto_Round_Point_Rotate_Event();
}

void ACommonBasePawn:: SpringArm_Camera_Round_Point_Rotate_Event_Implementation(const FVector Location,const FRotator Rotate,const float ViewDistance)
{
	//设置SpringArm的属性
	ForceConvertToSpingArmCameraSetProperty(Location,Rotate,ViewDistance);

	//设置累计未收到输入的时间 等于 开始自动旋转的时间
	Current_No_Action_Total_Time=Start_Auto_Rotate_Time;

	//开启绕点旋转
	bCan_AutoRotate=true;
}

#pragma region 输入事件输入相关
void ACommonBasePawn::ReceiveFVector2DInputEvent_Implementation(const FVector2D Value)
{
	//当前正在播放漫游Sequence动画时不接收输入
	if (bPlayingSequence) return;

	//当前正在进行视口混合时不接收输入
	if (bPlayingViewBlend) return;

	//根据移动方式选择性做特定的处理:仅移动不旋转
	if (bMove_Action_Type==true&&bRotate_Action_Type==false)
	{
		ReceiveFVector2DInputMoveEvent(Value);
		return;
	}

	//根据移动方式选择性做特定的处理:仅旋转不移动
	if (bMove_Action_Type==false&&bRotate_Action_Type==true)
	{
		ReceiveFVector2DInputRotateEvent(Value);
	}

	//根据移动方式选择性做特定的处理:仅不旋转也不移动
	if (bMove_Action_Type==false&&bRotate_Action_Type==false)
	{
		return;
	}

	//根据移动方式选择性做特定的处理:旋转加移动
	if (bMove_Action_Type==true&&bRotate_Action_Type==true)
	{
		return;
	}
	
	
	return;
	
}

void ACommonBasePawn::NoCameraReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value)
{
}

void ACommonBasePawn::AllCameraReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value)
{
}

void ACommonBasePawn::ReceiveFVector2DInputRotateEvent_Implementation(const FVector2D Value)
{
	//根据激活的相机做特定的处理:Normal_Camera相机激活,SpringArm_Camera相机未激活
	if (Normal_Camera->IsActive()&&!SpringArm_Camera->IsActive())
	{
		//执行Normal_Camera相机的移动
		NormalCameraReceiveFVector2DInputRotateEvent(Value);
		return;
	}
    
	//根据激活的相机做特定的处理:SpringArm_Camera相机激活,Normal_Camera相机未激活
	if (SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		//执行SpringArm_Camera相机的旋转
		SpringArmCameraReceiveFVector2DInputRotateEvent(Value);
		return;
	}
    
	//根据激活的相机做特定的处理:Normal_Camera相机未激活,SpringArm_Camera相机未激活
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		NoCameraReceiveFVector2DInputRotateEvent(Value);
		return;
	}

	//根据激活的相机做特定的处理:Normal_Camera相机和SpringArm_Camera相机都激活时
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		AllCameraReceiveFVector2DInputRotateEvent(Value);
		return;
	}
}

void ACommonBasePawn::ReceiveFVector2DInputMoveEvent_Implementation(const FVector2D Value)
{
	//根据激活的相机做特定的处理:Normal_Camera相机激活,SpringArm_Camera相机未激活
	if (Normal_Camera->IsActive()&&!SpringArm_Camera->IsActive())
	{
		//执行Normal_Camera相机的移动
		NormalCameraReceiveFVector2DInputMoveEvent(Value);
		return;
	}
    
	//根据激活的相机做特定的处理:SpringArm_Camera相机激活,Normal_Camera相机未激活
	if (SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		//执行SpringArm_Camera相机的移动
		SpringArmCameraReceiveFVector2DInputMoveEvent(Value);
		return;
	}
    
	//根据激活的相机做特定的处理:Normal_Camera相机未激活,SpringArm_Camera相机未激活
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		NoCameraReceiveFVector2DInputMoveEvent(Value);
		return;
	}

	//根据激活的相机做特定的处理:Normal_Camera相机和SpringArm_Camera相机都激活时
    if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
    {
    	AllCameraReceiveFVector2DInputMoveEvent(Value);
    	return;
    }
}

void ACommonBasePawn::SpringArm_Camera_Receive_FVector2D_Input_NotMoveAddRotate_Event_Implementation(
	const FVector2D& Value)
{
	UE_LOG(LogTemp,Warning,TEXT("此时处于不移动不旋转的状态,暂无具体执行"));
}

void ACommonBasePawn::SpringArm_Camera_Receive_FVector2D_Input_MoveAddRotate_Event_Implementation(
	const FVector2D& Value)
{
	UE_LOG(LogTemp,Warning,TEXT("此时处于移动和旋转的状态,暂无具体执行"));
}


void ACommonBasePawn::SpringArm_Camera_Receive_Float_Input_Event_Implementation(const float Value)
{
	//检测是否可拉近拉远视角
	if (!bCan_ScaleView) return;

	//当前正在播放漫游Sequence动画是退出
	if (bPlayingSequence) return;

	//当前正在进行视口混合时退出
	if (bPlayingViewBlend) return;
	
	//检测当前角度是否在限定范围内
	if (GetActorRotation().Pitch<Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.X||GetActorRotation().Pitch>Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.Y)
	{
		//不在限定范围内
		//GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("角度Pitch超出范围，无法缩放视角"));
		UE_LOG(LogTemp,Warning,TEXT("角度Pitch超出范围，无法缩放视角"));
		return;
	}

	//只要是SpringArm_Camera激活则尝试转换Camera
	if (!SpringArm_Camera->IsActive())
	{
		Normal_Camera_To_SpringArm_Camera();
		return;
	}

	//检查Normal_Camera是否为激活,Normal_Camera此时不应该为激活状态
	if (Normal_Camera->IsActive()) GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("Erro:Normal_Camera是激活的"));
	
	//弹簧臂的变化值
	const float SpringArmChange_Value=SpringArm->TargetArmLength*(0.2f)>1000.f?SpringArm->TargetArmLength*(0.2f)*Value:1000.f*Value;

	//当前的弹簧臂的值
	CurrentSpringArm_Length=SpringArm->TargetArmLength;
	
	//目标的弹簧臂长度
	TargetSpringArm_Length=UKismetMathLibrary::FClamp(SpringArm->TargetArmLength-SpringArmChange_Value,SpringArmLength_Limit.X,SpringArmLength_Limit.Y);
	
	//重置自动旋转时间
	Reset_Auto_Rotate_Total_Time();
	
	if (!SpringArm_TimeLine)
	{
		SpringArm_TimeLine=NewObject<UTimelineComponent>(this,TEXT("SpringArm_TimeLine"));
		SpringArm_TimeLine->RegisterComponent();
		SpringArm_TimeLine->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		SpringArm_TimeLine->SetLooping(false);
		SpringArm_TimeLine->SetTimelineLength(0.35f);
		
		//定义一个FOnTimelineEvent委托,用来绑定更新事件.
		FOnTimelineFloat OnTimelineEvent;
		OnTimelineEvent.BindUFunction (this,"SpringArm_TimeLine_Update");
		UCurveFloat* CurveFloat=NewObject<UCurveFloat>();
		CurveFloat->FloatCurve.UpdateOrAddKey(0,0);
		CurveFloat->FloatCurve.UpdateOrAddKey(0.35,1);
		SpringArm_TimeLine->AddInterpFloat(CurveFloat,OnTimelineEvent);
	}
	SpringArm_TimeLine->PlayFromStart();
}

#pragma endregion

void ACommonBasePawn::GetCurrentActiveCamera(TEnumAsByte<ECurrentActiveCamera>& CurrentActiveCamera)
{
	//Normal_Camera相机和SpringArm_Camera相机都激活时
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		CurrentActiveCamera=ECurrentActiveCamera::All_Camera_Active;
		return;
	}
	
	//当Normal_Camera相机激活,且SpringArm_Camera相机未激活时调用
	if (Normal_Camera->IsActive()&&!SpringArm_Camera->IsActive())
	{
		CurrentActiveCamera=ECurrentActiveCamera::Only_Normal_Camera_Active;
		return;
	}

	//当SpringArm_Camera相机激活,且Normal_Camera相机未激活时调用
	if (SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		CurrentActiveCamera=ECurrentActiveCamera::Only_SpringArm_Camera_Active;
		return;
	}

	//Normal_Camera相机和SpringArm_Camera相机都未激活时
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		CurrentActiveCamera=ECurrentActiveCamera::No_Camera_Active;
	}
}

void ACommonBasePawn::SpringArm_Camera_Receive_FVector2D_Input_Event_Implementation(const FVector2D& Value)
{
	//检测当前角度是否在限定范围内
	if (!(UKismetMathLibrary::InRange_FloatFloat(GetActorRotation().Pitch,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.X,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.Y),true,true))
	{
		//不在范围内,尝试转为Normal_Camera
		SpringArm_Camera_To_Normal_Camera();

		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
		
		return;
	}

	//检测此时是否处于不移动不旋转的状态
	if (bMove_Action_Type!=true&&bRotate_Action_Type!=true)
	{
		SpringArm_Camera_Receive_FVector2D_Input_NotMoveAddRotate_Event(Value);
	}

	
	//检测此时是否处于仅移动不旋转的状态
	if (bMove_Action_Type==true&&bRotate_Action_Type!=true)
	{
		SpringArmCameraReceiveFVector2DInputMoveEvent(Value);

		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}

	//检测此时是否处于仅旋转不移动的状态
	if (bMove_Action_Type!=true&&bRotate_Action_Type==true)
	{
		SpringArmCameraReceiveFVector2DInputRotateEvent(Value);
		
		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}

	//检测此时是否处于旋转和移动的状态
	if (bMove_Action_Type==true&&bRotate_Action_Type==true)
	{
		SpringArm_Camera_Receive_FVector2D_Input_MoveAddRotate_Event(Value);
	}
}

void ACommonBasePawn::Normal_Camera_Receive_FVector2D_Input_Event_Implementation(const FVector2D& Value)
{
	//仅鼠标左键按下
	if (bMove_Action_Type==true&&bRotate_Action_Type!=true)
	{
		
		//角度在限定范围内,可以从Normal_Camera转换SpringArm_Camera
		if (UKismetMathLibrary::InRange_FloatFloat(this->GetActorRotation().Pitch,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.X,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.Y,true,true))
		{
			
			//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,FString::SanitizeFloat(this->GetActorRotation().Pitch));

			//角度在范围内,可以从Normal_Camera转为SpringArm_Camera
			Normal_Camera_To_SpringArm_Camera();

			if (!SpringArm_Camera->IsActive()) UE_LOG(LogTemp,Warning,TEXT("Normal_Camera转换SpringArm_Camera失败"));
			
			//重置自动旋转时间
			Reset_Auto_Rotate_Total_Time();

			return;
		}
		else
		{
			//角度不在范围内,继续使用Normal_Camera的移动
			NormalCameraReceiveFVector2DInputMoveEvent(Value);

			//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera_移动"));
			
			//重置自动旋转时间
			Reset_Auto_Rotate_Total_Time();
			return;
		}
	}

	//仅鼠标右键按下
	if (bMove_Action_Type!=true&&bRotate_Action_Type==true)
	{
		//进行旋转操作
		NormalCameraReceiveFVector2DInputRotateEvent(Value);
		//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera_旋转"));
		
		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
		return;
	}

	//鼠标左键和鼠标右键都已按下
	if (bMove_Action_Type==true&&bRotate_Action_Type==true)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
		UE_LOG(LogTemp,Warning,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
		return;
	}
	
	
}





void ACommonBasePawn::Ensure_Only_One_Camera_Active_Internal()
{
	
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		if (Normal_Camera->IsActive())
		{
			//两个相机都激活,优先激活SpringArm_Camera
			Normal_Camera->SetActive(false);
		}
	}
	else
	{
		if (!Normal_Camera->IsActive())
		{
			//两个相机都为激活,优先激活SpringArm_Camera
			SpringArm_Camera->SetActive(true);
		}
	}
}

void ACommonBasePawn::OnOnlyMouseLeftButtonPressed_Internal(const FVector2D& Value)
{
	//仅当鼠标左键按下,且鼠标右键未按下时.
	if (bMove_Action_Type&&!bRotate_Action_Type)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bRotate_Action_Type&&!bMove_Action_Type)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bMove_Action_Type&&bRotate_Action_Type)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::OnOnlyMouseRightButtonPressed_Internal(const FVector2D& Value)
{
	//仅当鼠标左键按下,且鼠标右键未按下时.
	if (bMove_Action_Type&&!bRotate_Action_Type)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bRotate_Action_Type&&!bMove_Action_Type)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bMove_Action_Type&&bRotate_Action_Type)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::OnMouseAllButtonPressed_Internal(const FVector2D& Value)
{
	//仅当鼠标左键按下,且鼠标右键未按下时.
	if (bMove_Action_Type&&!bRotate_Action_Type)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bRotate_Action_Type&&!bMove_Action_Type)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bMove_Action_Type&&bRotate_Action_Type)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::SpringArm_TimeLine_Update(const float Value)
{
	const float SpringArm_Final_Length= UKismetMathLibrary::Lerp(CurrentSpringArm_Length,TargetSpringArm_Length,Value);
	SpringArm->TargetArmLength=SpringArm_Final_Length;
}

bool ACommonBasePawn::Can_Normal_Camera_To_SpringArm_Camera_Pitch_Range()
{
	return (UKismetMathLibrary::InRange_FloatFloat(GetActorRotation().Pitch,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.X,Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.Y),true,true);
}

void ACommonBasePawn::SpringArmCameraReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value)
{
	//当bCan_Move为false时退出.
	if (!bCan_Move) return;
	
	//检测bEnableCameraLag是否开启
	if (!SpringArm->bEnableCameraLag) SpringArm->bEnableCameraLag=true;

	//检测bEnableCameraRotationLag是否开启
	if (!SpringArm->bEnableCameraRotationLag) SpringArm->bEnableCameraRotationLag=true;
		
	
	//根据弹簧臂长度计算移动的值
	const float MoveValue=this->SpringArm->TargetArmLength*(0.008)>=50?this->SpringArm->TargetArmLength*(0.008):50;

	const APlayerCameraManager* PlayerCameraManager= UGameplayStatics::GetPlayerCameraManager(this,0);

	//计算在X方向上的移动距离.
	const FVector XVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetRightVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector X_Vector=XVectorNormal*(MoveValue*Value.X*SpringArm_Camera_Move_Param.X);
	
	//计算在Y方向上的移动距离.
	const FVector YVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector Y_Vector=YVectorNormal*(MoveValue*Value.Y*SpringArm_Camera_Move_Param.Y);

	//计算Pawn的最终位置
	const FVector PawnFinalLocation=GetActorLocation()+X_Vector+Y_Vector;

	//更新Pawn的位置.
	if (bCan_Limit_Move_Range)
	{
        SetActorLocation(FVector(
        		UKismetMathLibrary::FClamp(PawnFinalLocation.X,Min_Move_Range.X,Max_Move_Range.X),
        		UKismetMathLibrary::FClamp(PawnFinalLocation.Y,Min_Move_Range.Y,Max_Move_Range.Y),
        		PawnFinalLocation.Z));
	}
	else
	{
		SetActorLocation(PawnFinalLocation);
	}
}

void ACommonBasePawn::SpringArmCameraReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value)
{
	//当bCan_Rotate为false时退出.
	if (!bCan_Rotate) return;
	
	//检测bEnableCameraLag是否开启
	if (!SpringArm->bEnableCameraLag) SpringArm->bEnableCameraLag=true;

	//检测bEnableCameraRotationLag是否开启
	if (!SpringArm->bEnableCameraRotationLag) SpringArm->bEnableCameraRotationLag=true;
	
	//计算旋转的Y:Pitch值
	const float New_TemPitchValue=Value.Y*(SpringArm_Camera_Rotate_Param.Y)+GetActorRotation().Pitch;
	const float NewPitchValue=New_TemPitchValue>=SpringArm_Camera_Rotate_Pitch_Range.X?(New_TemPitchValue<=SpringArm_Camera_Rotate_Pitch_Range.Y?New_TemPitchValue:SpringArm_Camera_Rotate_Pitch_Range.Y):SpringArm_Camera_Rotate_Pitch_Range.X;

	//计算旋转的Z:Yaw值
	const float NewYawValue=Value.X*(SpringArm_Camera_Rotate_Param.X)+GetActorRotation().Yaw;

	//更新Pawn的角度.
	SetActorRotation(FRotator(
		NewPitchValue,
		NewYawValue,
		GetActorRotation().Roll));
}

void ACommonBasePawn::NormalCameraReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value)
{
	//当bCan_Rotate为false时退出.
	if (!bCan_Rotate) return;
	
	const FRotator CurrentRotation=GetActorRotation();
	
	//计算旋转的Y:Pitch值,限定Pitch的角度,因为Pitch的值为-90或90度时会有问题.
	const float New_Tem_Pitch_Value=Value.Y*(Normal_Camera_Rotate_Param.Y)+CurrentRotation.Pitch;
	const float NewPitchValue=New_Tem_Pitch_Value>=Normal_Camera_Rotate_Pitch_Range.X?(New_Tem_Pitch_Value<=Normal_Camera_Rotate_Pitch_Range.Y?New_Tem_Pitch_Value:Normal_Camera_Rotate_Pitch_Range.Y):Normal_Camera_Rotate_Pitch_Range.X;
	
	//计算旋转的Z:Yaw值
	const float NewYawValue=Value.X*(Normal_Camera_Rotate_Param.X)+CurrentRotation.Yaw;
	
	//更新Pawn的角度.
	SetActorRotation(FRotator(NewPitchValue,NewYawValue,CurrentRotation.Roll));
}

void ACommonBasePawn::NormalCameraReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value)
{
	//当bCan_Move为false时退出.
	if (!bCan_Move) return;
	
	//根据弹簧臂长度计算移动的值
	const float MoveValue=this->SpringArm->TargetArmLength*(0.005)>=50?this->SpringArm->TargetArmLength*(0.005):50;

	const APlayerCameraManager* PlayerCameraManager= UGameplayStatics::GetPlayerCameraManager(this,0);

	//计算在X方向上的移动距离.
	const FVector XVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetRightVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector XVector=XVectorNormal*(MoveValue*Value.X*Normal_Camera_Move_Param.X);
	
	//计算在Y方向上的移动距离.
	const FVector YVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector YVector=YVectorNormal*(MoveValue*Value.Y*Normal_Camera_Move_Param.Y);

	//计算Pawn的最终位置
	const FVector PawnFinalLocation=GetActorLocation()+XVector+YVector;

	//更新Pawn的位置.
	if (bCan_Limit_Move_Range)
	{
		SetActorLocation(FVector(
				UKismetMathLibrary::FClamp(PawnFinalLocation.X,Min_Move_Range.X,Max_Move_Range.X),
				UKismetMathLibrary::FClamp(PawnFinalLocation.Y,Min_Move_Range.Y,Max_Move_Range.Y),
				PawnFinalLocation.Z));
	}
	else
	{
		SetActorLocation(PawnFinalLocation);
	}
	
}

void ACommonBasePawn::Normal_Camera_To_SpringArm_Camera_Implementation()
{
	//检测SpringArm_Camera相机当前是否激活
	if (SpringArm_Camera->IsActive())
	{
		return ;
	}
	
	const FRotator CurrentCameraRotate=GetActorRotation();	//获取当前的方向(此时方向应该与Norma_Camera一致)
	const FVector CurrentCameraLoc=GetActorLocation();		//获取当前的位置(此时位置应该与Norma_Camera一致)

	//检查当前角度的Pitch值，是否处于限定范围内
	if (CurrentCameraRotate.Pitch<Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.X||CurrentCameraRotate.Pitch>Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range.Y)
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Erro:Z轴位置错误,超出限定范围,无法切换相机"));
		UE_LOG(LogTemp,Warning,TEXT("Erro:Z轴位置错误,超出限定范围,无法切换相机"));
		return ;
	}
	
	//检查当前位置的Z轴高度，是否处于限定范围
	if (CurrentCameraLoc.Z<Pawn_Min_Z_Value)				
	{
		//GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Erro:相机角度出现错误,超过限定范围,无法切换相机"));
		UE_LOG(LogTemp,Warning,TEXT("Erro:相机角度出现错误,超过限定范围,无法切换相机"))
		return ;
	}

	//计算最终Pawn的位置，并计算弹簧臂应该设定的长度
	const float Camera_Vertical_Value=CurrentCameraLoc.Z-Pawn_Min_Z_Value;							//相机的垂直方向长度
	const double DegreeValue=FMath::Abs(UKismetMathLibrary::DegSin(CurrentCameraRotate.Pitch));	//求垂直边于斜边的比例
	const float FinalSpringArmLength=Camera_Vertical_Value/DegreeValue;								//最终的斜边长度（弹簧臂长度）

	//更新设置弹簧臂的长度
	this->SpringArm->TargetArmLength=FinalSpringArmLength;

	//计算Pawn最终的位置
	const FVector ForwardVector=UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(CurrentCameraRotate));
	const FVector FinalLocation=FinalSpringArmLength*ForwardVector+CurrentCameraLoc;

	//更新Pawn的位置
	SetActorLocation(FinalLocation);

	//位置都已计算完毕,开始切换相机.
	SpringArm_Camera->SetActive(true);	//激活弹簧臂相机
	Normal_Camera->SetActive(false);	//灭活普通相机
}

void ACommonBasePawn::ForceConvertToSpingArmCameraSetProperty_Implementation(
	const FVector Location,
	const FRotator Rotate,
	const float ViewDistance)
{
	//强制转换确保现在使用的是SpringArm_Camera
    ForceConvertToSpringArmCamera();
    	
    //设置弹簧臂的长度
    SpringArm->TargetArmLength=ViewDistance;
    
    //设置位置和方向
    SetActorLocation(Location);
    SetActorRotation(Rotate);
}

void ACommonBasePawn::ForceConvertToSpringArmCamera_Implementation()
{
	//先检查当前的是否为SpringArm_Camera,如果不是则第一次尝试使用方法转换
	if (SpringArm_Camera->IsActive()) return;

	//尝试使用方法转换
	Normal_Camera_To_SpringArm_Camera();

	//再检查当前的是否为SpringArm_Camera,如果还不是则直接转换
	if (!SpringArm_Camera->IsActive())
	{
		SpringArm_Camera->SetActive(true);	//激活弹簧臂相机
		Normal_Camera->SetActive(false);	//灭活普通相机
	}
}

void ACommonBasePawn::Set_Camera_View_Internal_Implementation(const FVector& InVector, const FRotator& InRotator)
{
	//确保切换到Normal_Camera
	if (!SpringArm_Camera_To_Normal_Camera())	return;
	
	//更新位置和角度
	SetActorLocation(InVector);
	SetActorRotation(InRotator);
}

void ACommonBasePawn::ViewBlend_TimeLine_Update_Implementation(const float Value)
{
	//更新位置
	SetActorLocation(UKismetMathLibrary::VLerp(BlendViewStartLocation,BlendViewTargetLocation,Value));

	//更新角度
	SetActorRotation(UKismetMathLibrary::RLerp(BlendViewStartRotate,BlendViewTargetRotate,Value,true),ETeleportType::None);

	//更新两个相机的FOV
	Normal_Camera->FieldOfView=UKismetMathLibrary::Lerp(FOVStart,FOVEnd,Value);
	SpringArm_Camera->FieldOfView=UKismetMathLibrary::Lerp(FOVStart,FOVEnd,Value);
}

bool ACommonBasePawn::SpringArm_Camera_To_Normal_Camera_Implementation()
{
	//检查普通的相机是否激活，激活则不需要操作。只有从弹簧臂相机到普通相机才需要。
	if (Normal_Camera->IsActive())	return true;
	
	//归零普通相机的相对旋转，只有这样普通相机使用的才是Pawn的角度
	this->Normal_Camera->SetRelativeTransform(FTransform());

	//取消相机Lag和归零弹簧比的长度
	this->SpringArm->TargetArmLength=0;
	this->SpringArm->bEnableCameraLag=false;
	
	//开始切换相机.
	Normal_Camera->SetActive(true);	//灭活弹簧臂相机
	SpringArm_Camera->SetActive(false);	//激活弹簧臂相机
	return true;
}

void ACommonBasePawn::Auto_Round_Point_Rotate_Event_Implementation()
{
	//检测是否开启自动旋转
	if (!bCan_AutoRotate)
	{
		Current_No_Action_Total_Time=0.f;
		return;
	}
	
	//检测当前是否正在播放漫游动画
	if (bPlayingSequence)
	{
		Current_No_Action_Total_Time=0.f;
		return;
	}

	//检测当前是否正在进行视角混合
	if (bPlayingViewBlend)
	{
		Current_No_Action_Total_Time=0.f;
		return;
	}

	//检测当前使用的相机是否为SpringArm_Camera
	if (!SpringArm_Camera->IsActive())
	{
		//当前没有激活SpringArm_Camera则需要转换
		if (Normal_Camera->IsActive())
		{
			Normal_Camera_To_SpringArm_Camera();
		}
		return;
	}
	
	//监测未操作时间是否大于设定值
	if (Current_No_Action_Total_Time<Start_Auto_Rotate_Time)
	{
		Current_No_Action_Total_Time+=UGameplayStatics::GetWorldDeltaSeconds(this);
		return;
	}
	
	//每帧的转动速度
	const float PerTick_Auto_Rotate_Speed=Auto_Rotate_Speed*UGameplayStatics::GetWorldDeltaSeconds(this);

	//旋转,更新角度
	SpringArmCameraReceiveFVector2DInputRotateEvent(FVector2D(PerTick_Auto_Rotate_Speed,0));
}

void ACommonBasePawn::Reset_Auto_Rotate_Total_Time_Implementation()
{
	Current_No_Action_Total_Time=0.f;
}

void ACommonBasePawn::Set_Camera_View_Implementation(
	const FVector& InVector,
	const FRotator& InRotator,
	const float CameraFOV)
{
	//确保切换到Normal_Camera
	if (!SpringArm_Camera_To_Normal_Camera())	return;

	//设置FOV
	Normal_Camera->FieldOfView=CameraFOV;
	SpringArm_Camera->FieldOfView=CameraFOV;
	
	//更新位置和角度
	SetActorLocation(InVector);
	SetActorRotation(InRotator);
}

void ACommonBasePawn::ViewBlendByTimeLine_Implementation(
	const FVector& InVector,
	const FRotator& InRotator,
	const float FOV,
	const float BlendTime)
{
	//确保用于视口混合的TimeLine有效
	if (!ViewBlend_TimeLine)
	{
		ViewBlend_TimeLine=NewObject<UTimelineComponent>(this,TEXT("ViewBlend_TimeLine"));
		ViewBlend_TimeLine->RegisterComponent();
		ViewBlend_TimeLine->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
		ViewBlend_TimeLine->SetLooping(false);
		ViewBlend_TimeLine->SetTimelineLength(1.f);

		//定义一个FOnTimelineEvent委托,用来绑定更新事件.
		FOnTimelineFloat OnTimelineEvent;
		OnTimelineEvent.BindUFunction(this,"ViewBlend_TimeLine_Update");
		UCurveFloat* CurveFloat=NewObject<UCurveFloat>();
		CurveFloat->FloatCurve.UpdateOrAddKey(0,0);
		CurveFloat->FloatCurve.UpdateOrAddKey(1,1);
		ViewBlend_TimeLine->AddInterpFloat(CurveFloat,OnTimelineEvent);

		FOnTimelineEvent OnTimelineFinishedEvent;
		OnTimelineFinishedEvent.BindUFunction(this,"ViewBlend_TimeLine_End");
		ViewBlend_TimeLine->SetTimelineFinishedFunc(OnTimelineFinishedEvent);
	}
	//设置播放速率
	ViewBlend_TimeLine->SetPlayRate(1/(BlendTime!=0?BlendTime:0.01));

	//记录开始的位置和角度
	BlendViewStartLocation=UGameplayStatics::GetPlayerCameraManager(this,0)->GetCameraLocation();
	BlendViewStartRotate=UGameplayStatics::GetPlayerCameraManager(this,0)->GetCameraRotation();
	
	//记录目标的位置和角度
	BlendViewTargetLocation=InVector;
	BlendViewTargetRotate=InRotator;

	//视角混合需要使用Normal_Camera
	if (!Normal_Camera->IsActive())
	{
		SpringArm_Camera_To_Normal_Camera();
	}

	//记录开始的FOV
	FOVStart=Normal_Camera->FieldOfView;

	//记录最终FOV
	FOVEnd=FOV;
	
	//用于定义当前处于视口混合状态
	bPlayingViewBlend=true;

	//重置自动旋转时间
	Reset_Auto_Rotate_Total_Time();
	
	//播放TimeLine
	ViewBlend_TimeLine->PlayFromStart();
}

void ACommonBasePawn::ViewBlend_TimeLine_End_Implementation()
{
	//将bPlayingViewBlend设置为false表示不处于视口混合过程中
	if (bPlayingViewBlend) bPlayingViewBlend=false;
}

void ACommonBasePawn::NoCameraReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value)
{
	UE_LOG(LogTemp,Warning,TEXT("Erro：Pawn上没有激活的Camera"));
}

void ACommonBasePawn::AllCameraReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value)
{
	GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("Erro：Pawn的Camera都已激活,这将出现逻辑错误"));
	UE_LOG(LogTemp,Warning,TEXT("Erro：Pawn的Camera都已激活,这将出现逻辑错误"));
}