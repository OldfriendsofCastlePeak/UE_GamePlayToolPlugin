// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "CommonGamePlayToolPlugin/Component/ViewToolComponent.h"
#include "GameFramework/Pawn.h"
#include "CommonBasePawn.generated.h"

UCLASS(Blueprintable,BlueprintType,HideCategories=(Pawn,Actor,Compomemts,UObject,Camera,Replication,Collision,HLOD,Input,Physics,DataLayers))
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBasePawn : public APawn
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

#pragma region Pawn默认的相关组件
	/* Actor的根组件 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	USceneComponent* Scene;

	/* 弹簧臂组件,用于鸟瞰形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	class USpringArmComponent* SpringArm;

	/* 挂在弹簧臂上的相机,用于鸟瞰形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	class UCameraComponent* SpringArm_Camera;

	/* 用于第一人称世界形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	UCameraComponent* Normal_Camera;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	UViewToolComponent* ViewToolComponent; 
	
#pragma endregion
	
#pragma region 处理相关功能需要的属性
	/* bL_Mouse_Pressed用于条件判断,用于当鼠标左键按下时才执行的行为 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Mouse")
	bool bL_Mouse_Pressed=false;

	/* bR_Mouse_Pressed用于条件判断,用于当鼠标右键按下时才执行的行为 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Mouse")
	bool bR_Mouse_Pressed=false;

	/* bCan_Move用于条件判断,控制Pawn是否可以在接收输入之后进行移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move")
	bool bCan_Move=true;

	/* 是否允许当一段时间未输入时自动旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate")
	bool bCan_AutoRotate=true;

	/* 自动速度,以秒为单位 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float Auto_Rotate_Speed=2;

	/* 设置的开始自动旋转时间,当超过该时间时,将自动旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float Start_Auto_Rotate_Time=15;
	
	/* 用于累计未输入的时间 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|AutoRotate",meta=(EditCondition="bCan_AutoRotate"))
	float No_Action_Total_Time=0;
	
	/* 是否限制Pawn的移动范围 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move")
	bool bCan_Limit_Move_Range=true;

	/* 最小的移动范围点 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move",meta=(EditCondition="bCan_Limit_Move_Range"))
	FVector2D Min_Move_Range=FVector2D(-1000000,-1000000);

	/* 最大的移动范围点 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move",meta=(EditCondition="bCan_Limit_Move_Range"))
	FVector2D Max_Move_Range=FVector2D(1000000,1000000);

	/* Pawn的原点在Z轴上可以到的最底值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move")
	float Pawn_Min_Z_Value=500;

	/* SpringArm_Camera旋转Pitch值范围,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	FVector2D SpringArm_Camera_Rotate_Pitch_Range=FVector2D(-80.f,-1.f);

	/* Normal_Camera旋转Pitch值范围,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Rotate_Pitch_Range=FVector2D(-89.f,89.f);
	
	/* 属性表示当前是否正在播放漫游Sequence动画 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move")
	bool bPlayingSequence=false;

	/* 属性表示当前是否正在进行视角混合 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Move")
	bool bPlayingViewBlend=false;

	/* 属性表示弹簧臂的长度的限定值,X值表示最小值,Y值表示最大值 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	FVector2D SpringArmLength_Limit=FVector2D(0.f,2000000.f);

	/* 属性存储UTimelineComponent,用于视角拉近和缩远的过渡 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|SpringArm_Camera")
	class UTimelineComponent* SpringArm_TimeLine;

	/* 属性Normal_Camera旋转参数,X:表示X方向的旋转,Y:表示Y方向的旋转 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Rotate_Param=FVector2D(0.5f,0.5f);

	/* 属性Normal_Camera移动参数,X:表示X方向的移动,Y:表示Y方向的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Normal_Camera")
	FVector2D Normal_Camera_Move_Param=FVector2D(-1.f,-1.f);
	
	/* 用于视口混合的临时Actor */
	UPROPERTY(Transient)
	AActor* View_Blend_Tem_Actor;
	
	/* 目标的弹簧臂长度,用于计算使用 */
	float TargetSpringArm_Length=0.f;
	
#pragma endregion

#pragma region 输入事件输入相关
	/* 用来存放UInputMappingContext的TMap,使用一个UInputMappingContext即可,为了扩展考虑,这里使用TMap */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	TMap<FName,UInputMappingContext*> InputMappingContextMap;

	/* 用来存放UInputAction的TMap */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	TMap<FName,UInputAction*> InputActionMap;

	/* 鼠标XY轴移动事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseXYMoveEvent(const FVector2D Value);

	/* 键盘按键WASD移动事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void KeyboardWASDMoveEvent(const FVector2D Value);

	/* 键盘按键EQ委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void KeyboardEQUpEvent(const float Value);

	/* 鼠标古滚轮委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseWheelEvent(const float Value);

	/* 鼠标左键委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseLeftButtonEvent(const bool Value);

	/* 鼠标右键委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseRightButtonEvent(const bool Value);

	/* 添加增强输入映射 */
	UFUNCTION(Blueprintable,Category="ACommonBasePawn|Input")
	void AddEnhancedContext();
	
private:
	void MouseXYMove_Internal(const FInputActionInstance& Value);
	void KeyboardWASDMove_Internal(const FInputActionInstance& Value);
	void KeyboardEQUp_Internal(const FInputActionInstance& Value);
	void MouseWheel_Internal(const FInputActionInstance& Value);
	void MouseLeft_Internal(const FInputActionInstance& Value);
	void MouseRight_Internal(const FInputActionInstance& Value);
#pragma endregion

#pragma region 处理Pawn移动相关
	/* 当SpringArm_Camera相机激活,且Normal_Camera相机未激活时调用 */
	void OnOnlySpringArm_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机激活,且SpringArm_Camera相机未激活时调用 */
	void OnOnlyNormal_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机和SpringArm_Camera相机都未激活时调用 */
	void OnNo_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机和SpringArm_Camera相机都激活时调用 */
	void OnAll_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value);

	/* 用于确保只有一个Camera处于激活状态 */
	void Ensure_Only_One_Camera_Active_Internal();
	
	/* 仅当鼠标左键按下,且鼠标右键未按下时调用 */
	void OnOnlyMouseLeftButtonPressed_Internal(const FVector2D& Value);

	/* 仅当鼠标右键按下,且鼠标左键未按下时调用 */
	void OnOnlyMouseRightButtonPressed_Internal(const FVector2D& Value);

	/* 当鼠标左键与右键都按下时调用 */
	void OnMouseAllButtonPressed_Internal(const FVector2D& Value);

	/* TimeLine绑定函数,用于弹簧臂长度的改变时的过渡效果 */
	UFUNCTION()
	void SpringArm_TimeLine_Update();
	
public:
	/* 用于使用弹簧臂相机时的Pawn移动 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|SpringArmCamera")
	void SpringArm_Camera_Move_Pawn(const FVector2D& Value);

	/* 用于使用弹簧臂相机时的Pawn旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|SpringArmCamera")
	void SpringArm_Camera_Rotate_Pawn(const FVector2D& Value);

	/* 用于使用普通相机时的Pawn移动 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalCamera")
	void Normal_Camera_Move_Pawn(const FVector2D& Value);

	/* 用于使用普通相机时的Pawn旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	void Normal_Camera_Rotate_Pawn(const FVector2D& Value);

	/* 用于将普通相机转为弹簧臂相机 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	bool Normal_Camera_To_SpringArm_Camera();

	/* 用于将弹簧臂相机转为普通相机 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	bool SpringArm_Camera_To_Normal_Camera();

	/* Pawn围绕当前点位旋转 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	void Pawn_Round_Event();

	/* 重置自动旋转总得计时时间 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	void Reset_Auto_Rotate_Total_Time();

	/* 直接设置视口的位置角度 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	void Set_Camera_View_By_Actor(const FVector& InVector,const FRotator& InRotator);

	/* 进行视角混合到特定的位置、角度、有过渡动画,可以使用UViewToolComponent组件获取结束时的委托 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBasePawn|Camera|NormalArmCamera")
	void Blend_View_Event(const FVector& InVector,const FRotator& InRotator,const float BlendTime=1.f,EViewTargetBlendFunction BlendFunc = VTBlend_Linear, float BlendExp = 1.f, bool bLockOutgoing = true);

	
#pragma endregion

	
};
