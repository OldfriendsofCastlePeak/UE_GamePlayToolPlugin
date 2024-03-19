// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CustomBasePawn.h"
#include "../../Component/ViewBlend/ViewBlendComponent.h"
#include "GameFramework/Pawn.h"
#include "Curves/RichCurve.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CommonBasePawn.generated.h"


UENUM(BlueprintType)
enum ECurrentActiveCamera:uint8
{
	All_Camera_Active,				//所有的相机都处于激活状态
	Only_SpringArm_Camera_Active,	//只有SpringArm_Camera相机处于激活状态
	Only_Normal_Camera_Active,		//只有Normal_Camera相机处于激活状态
	No_Camera_Active				//没有相机处于激活状态
};

UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBasePawn : public ACustomBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommonBasePawn(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 该方法用于在SetupPlayerInputComponent方法中调用,以供蓝图扩展调用 */
	UFUNCTION(BlueprintNativeEvent,Category="ACommonBasePawn")
	void SetupPlayerInputComponent_Internal();

	/* 仅用于测试显示当前可识别的输入 */
	UFUNCTION(BlueprintPure,meta=(DevelopmentOnly),Category="ACommonBasePawn")
	int32 GetAllInput();
	
	/* 用于在Tick方法中使用的计时操作 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void TickTimerEvent(float DeltaTime);
	








	
#pragma region Pawn默认的相关组件
public:	
	/* Pawn的根组件 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn")
	USceneComponent* Scene;

	/* 弹簧臂组件,用于鸟瞰形式的移动 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn")
	USpringArmComponent* SpringArm;

	/* 挂在弹簧臂上的相机,用于鸟瞰形式的移动 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn")
	UCameraComponent* SpringArm_Camera;

	/* 用于第一人称形式的移动 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn")
	UCameraComponent* Normal_Camera;

	/* 属性存储UTimelineComponent,用于弹簧臂拉近和缩远变化的过渡 */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	UTimelineComponent* SpringArm_TimeLine;

	/* 用于视口混合时使用的Blend */
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	UTimelineComponent* ViewBlend_TimeLine;

	/* 用于进行FOV差值的开始和结束值。 */
	float FOVStart,FOVEnd;
	
#pragma endregion

#pragma region 开关属性
public:	
	/* bCan_Move用于条件判断,控制Pawn是否可以在接收输入之后进行移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Switch")
	bool bCan_Move=true;

	/* bCan_Rotate用于条件判断,控制Pawn是否可以在接收输入之后进行旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Switch")
	bool bCan_Rotate=true;

	/* bCan_BreakSequencePlay用于条件判断,控制Pawn是否可以在播放Sequence时接收输入之后中断Sequence播放 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Switch")
	bool bCan_BreakSequencePlay=false;
	
	/* 是否允许当一段时间未输入时自动旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Switch")
	bool bCan_AutoRotate=true;

	/* 用户设置是否允许拉近和拉远视角 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Switch")
	bool bCan_ScaleView=true;
#pragma endregion

#pragma region 状态属性
public:	
	/* 属性表示当前是否正在播放漫游Sequence动画 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|State")
	bool bPlayingSequence=false;

	/* 属性表示当前是否正在进行视角混合 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|State")
	bool bPlayingViewBlend=false;
#pragma endregion

#pragma region 自动绕点旋转属性
	/* 自动速度,以秒旋转的度数 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float Auto_Rotate_Speed=2;

	/* 设置的开始自动旋转时间,当超过该时间时,将自动旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float Start_Auto_Rotate_Time=15;
	
	/* 当前累计未收到输入的时间 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float Current_No_Action_Total_Time=0;
	
	/* Pawn围绕当前点位旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|AutoRotate")
	void Auto_Round_Point_Rotate_Event();

	/* 给定一个位置、角度,SpringArm_Camera将绕其旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|AutoRotate")
	void SpringArm_Camera_Round_Point_Rotate_Event(const FVector Location,const FRotator Rotate,const float ViewDistance=3000.f);
	
	/* 重置自动旋转总得计时时间 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|AutoRotate")
	void Reset_Auto_Rotate_Total_Time();
#pragma endregion

#pragma region 限制属性
	/* 是否限制Pawn的移动范围 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit")
	bool bCan_Limit_Move_Range=true;

	/* 最小的移动范围点 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit",meta=(EditCondition="bCan_Limit_Move_Range"))
	FVector2D Min_Move_Range=FVector2D(-1000000,-1000000);

	/* 最大的移动范围点 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit",meta=(EditCondition="bCan_Limit_Move_Range"))
	FVector2D Max_Move_Range=FVector2D(1000000,1000000);
	
	/* Pawn的原点在Z轴上可以到的最底值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit")
	float Pawn_Min_Z_Value=500;

	/* 是否限制SpringArm的弹簧臂属性的最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit")
	bool bEnableLimietSpringArmLength=true;

	/* 属性表示弹簧臂的长度的限定值,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Limit",meta=(EditCondition="bEnableLimietSpringArmLength"))
	FVector2D SpringArmLength_Limit=FVector2D(0.f,2000000.f);
#pragma endregion

#pragma region SpringArm_Camera移动
	/* SpringArm_Camera旋转Pitch值范围,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	FVector2D SpringArm_Camera_Rotate_Pitch_Range=FVector2D(-80.f,-1.f);
	
	/* 属性SpringArm_Camera旋转系数,X:表示X方向的旋转,Y:表示Y方向的旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	FVector2D SpringArm_Camera_Rotate_Param=FVector2D(1.f,1.f);
	
	/* 属性SpringArm_Camera移动系数,X:表示X方向的移动,Y:表示Y方向的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	FVector2D SpringArm_Camera_Move_Param=FVector2D(-1.f,-1.f);
#pragma endregion
             
#pragma region Normal_Camera
	/* Normal_Camera可以转为SpringArm_Camera时的Pitch值范围,X值表示最小值,Y值表示最大值,最好不要更改 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_To_SpringArm_Camera_Rotate_Pitch_Range=FVector2D(-80.f,-1.f);
	
	/* Normal_Camera旋转Pitch值范围,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Rotate_Pitch_Range=FVector2D(-89.f,89.f);

	/* 属性Normal_Camera旋转参数,X:表示X方向的旋转,Y:表示Y方向的旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Rotate_Param=FVector2D(0.5f,0.5f);
	
	/* 属性Normal_Camera移动参数,X:表示X方向的移动,Y:表示Y方向的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Move_Param=FVector2D(-1.f,-1.f);

#pragma endregion

#pragma region 处理相关功能需要的属性
public:	
	/* 用于视口混合的临时Actor */
	UPROPERTY()
	ABlendViewPawn* View_Blend_Tem_Pawn;

	/* 目标的弹簧臂长度,用于计算使用 */
	float CurrentSpringArm_Length=0.f;
	
	/* 目标的弹簧臂长度,用于计算使用 */
	float TargetSpringArm_Length=0.f;
	
#pragma endregion

#pragma region 操作方式
public:	
	/* 当bMove_Action_Type为true时,收到FVector2D输入将进行移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Type")
	bool bMove_Action_Type=false;
    
	/* 当bRotate_Action_Type为true时,收到FVector2D输入将进行旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Type")
	bool bRotate_Action_Type=false;
#pragma endregion

#pragma region 接收输入
	/* 接收FVector2D输入来进行移动或旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void ReceiveFVector2DInputEvent(const FVector2D Value);
	virtual void ReceiveFVector2DInputEvent_Implementation(const FVector2D Value);

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/* 接收FVector2D输入来进行移动 */
	//UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	virtual void ReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value) override;
	
	/* SpringArm_Camera接收FVector2D输入来进行移动 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera")
	void SpringArmCameraReceiveFVector2DInputMoveEvent(const FVector2D& Value);
	
	/* 用于使用普通相机时的Pawn移动 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Normal_Camera")
	void NormalCameraReceiveFVector2DInputMoveEvent(const FVector2D& Value);

	/* 当Normal_Camera相机和SpringArm_Camera相机都未激活时调用 */
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
    void NoCameraReceiveFVector2DInputMoveEvent(const FVector2D& Value);
    	
    /* 当Normal_Camera相机和SpringArm_Camera相机都激活时调用 */
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
    void AllCameraReceiveFVector2DInputMoveEvent(const FVector2D& Value);
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	

	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/* 接收FVector2D输入来进行旋转 */
    //UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
    virtual void ReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value) override;
	
	/* SpringArm_Camera接收FVector2D输入来进行旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera")
	void SpringArmCameraReceiveFVector2DInputRotateEvent(const FVector2D& Value);

	/* 用于使用普通相机时的Pawn旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Normal_Camera")
	void NormalCameraReceiveFVector2DInputRotateEvent(const FVector2D& Value);
	
	/* 当Normal_Camera相机和SpringArm_Camera相机都未激活时调用 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void NoCameraReceiveFVector2DInputRotateEvent(const FVector2D& Value);
    
	/* 当Normal_Camera相机和SpringArm_Camera相机都激活时调用 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void AllCameraReceiveFVector2DInputRotateEvent(const FVector2D& Value);
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////
	
	/* SpringArm_Camera接收FVector2D输入来进行移动+旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera")
	void SpringArm_Camera_Receive_FVector2D_Input_MoveAddRotate_Event(const FVector2D& Value);

	/* SpringArm_Camera接收FVector2D输入来进行不进行移动+旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera")
	void SpringArm_Camera_Receive_FVector2D_Input_NotMoveAddRotate_Event(const FVector2D& Value);
	
	/* SpringArm_Camera接收Float输入来进行缩放弹簧臂 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera")
	void SpringArm_Camera_Receive_Float_Input_Event(const float Value);
	
	/* SpringArm_Camera接收FVector2D输入来进行移动或旋转 */
    UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|SpringArm_Camera",meta=(DeprecatedFunction))
    void SpringArm_Camera_Receive_FVector2D_Input_Event(const FVector2D& Value);
	
	/* Normal_Camera接收FVector2D输入来进行移动或旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Normal_Camera",meta=(DeprecatedFunction))
	void Normal_Camera_Receive_FVector2D_Input_Event(const FVector2D& Value);

#pragma endregion

#pragma region 处理Pawn移动相关
	/* 获取当前处于激活状态的相机状态 */
	UFUNCTION(BlueprintCallable,meta=(ExpandEnumAsExecs="CurrentActiveCamera"),Category="ACommonBasePawn")
	void GetCurrentActiveCamera(TEnumAsByte<ECurrentActiveCamera>& CurrentActiveCamera);
	
	/* 用于确保只有一个Camera处于激活状态 */
	UFUNCTION(BlueprintCallable,Category="ACommonBasePawn")
	void Ensure_Only_One_Camera_Active_Internal();
	
	/* 仅当鼠标左键按下,且鼠标右键未按下时调用 */
	UFUNCTION(BlueprintCallable,Category="ACommonBasePawn")
	void OnOnlyMouseLeftButtonPressed_Internal(const FVector2D& Value);

	/* 仅当鼠标右键按下,且鼠标左键未按下时调用 */
	UFUNCTION(BlueprintCallable,Category="ACommonBasePawn")
	void OnOnlyMouseRightButtonPressed_Internal(const FVector2D& Value);

	/* 当鼠标左键与右键都按下时调用 */
	UFUNCTION(BlueprintCallable,Category="ACommonBasePawn")
	void OnMouseAllButtonPressed_Internal(const FVector2D& Value);

	/* TimeLine绑定函数,用于弹簧臂长度的改变时的过渡效果 */
	UFUNCTION()
	void SpringArm_TimeLine_Update(const float Value);
#pragma endregion

#pragma region 相机转换
public:	
	/* 检测当前视口角度是否处于可以从Normal_Camera转为SpingArm_Camera的Pitch角度范围内 */
	UFUNCTION(BlueprintPure,Category="ACommonBasePawn|Camera")
	bool Can_Normal_Camera_To_SpringArm_Camera_Pitch_Range();
	
	/* 用于将普通相机转为弹簧臂相机 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Convert")
	void Normal_Camera_To_SpringArm_Camera();
	virtual void Normal_Camera_To_SpringArm_Camera_Implementation();

	/* 用于将弹簧臂相机转为普通相机 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Convert")
	bool SpringArm_Camera_To_Normal_Camera();

	/* 强制转换当前的相机为SpringArm_Camera */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Convert")
	void ForceConvertToSpringArmCamera();

	/* 强制装换当前的相机为SpringArm_Camera并设置其位置和方向 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="Convert")
	void ForceConvertToSpingArmCameraSetProperty(const FVector Location,const FRotator Rotate,const float ViewDistance=3000.f);
	
#pragma endregion

#pragma region 视口过渡
	/* 用于记录视角混合的镜头 */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn")
	FName BlendViewCameraName;

	/* 用于记录视口需要混合到的位置 */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn")
	FVector BlendViewTargetLocation;

	/* 用于记录视口需要混合到的角度 */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn")
	FVector BlendViewStartLocation;
	
	/* 用于记录视口需要混合到的角度 */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn")
	FRotator BlendViewTargetRotate;

	/* 用于记录视口需要混合到的角度 */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn")
	FRotator BlendViewStartRotate;
	
	/* 直接设置视口的位置、角度、镜头FOV */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void Set_Camera_View(const FVector& InVector,const FRotator& InRotator,const float CameraFOV=90);

	/* 直接设置视口的位置角度无FOV版本 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void Set_Camera_View_Internal(const FVector& InVector,const FRotator& InRotator);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void ViewBlendByTimeLine(const FVector& InVector,const FRotator& InRotator,const float FOV=90.f,const float BlendTime=1.f);
	
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void ViewBlend_TimeLine_Update(const float Value);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn")
	void ViewBlend_TimeLine_End();

#pragma endregion




	
	
};
