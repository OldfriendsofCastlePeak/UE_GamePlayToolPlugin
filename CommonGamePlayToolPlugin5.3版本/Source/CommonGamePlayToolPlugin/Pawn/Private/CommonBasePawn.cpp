// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CommonBasePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/SpringArmComponent.h"
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
	SpringArm_Camera->SetActive(true);
	
	//创建用于第一人称世界形式的移动的相机.
	Normal_Camera=CreateDefaultSubobject<UCameraComponent>("Normal_Camera");
	Normal_Camera->SetupAttachment(Scene);
	SpringArm_Camera->SetActive(false);

	//创建用于视口混合的ViewToolComponent组件
	ViewToolComponent=CreateDefaultSubobject<UViewToolComponent>("ViewToolComponent");
	
	//IA_Float2D_Mouse用来处理鼠标在XY轴上的输入
	UInputAction* IA_Float2D_Mouse=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float2D_Mouse"));
	IA_Float2D_Mouse->bConsumeInput=true;
	IA_Float2D_Mouse->bTriggerWhenPaused=false;
	IA_Float2D_Mouse->bReserveAllMappings=false;
	IA_Float2D_Mouse->ValueType=EInputActionValueType::Axis2D;
	InputActionMap.Add(TEXT("IA_Float2D_Mouse"),IA_Float2D_Mouse);

	//IA_Bool_MouseLeft用来处理鼠标左键的输入
	UInputAction* IA_Bool_MouseLeft=CreateDefaultSubobject<UInputAction>(TEXT("IA_Bool_MouseLeft"));
    IA_Bool_MouseLeft->bConsumeInput=true;
    IA_Bool_MouseLeft->bTriggerWhenPaused=false;
    IA_Bool_MouseLeft->bReserveAllMappings=false;
    IA_Bool_MouseLeft->ValueType=EInputActionValueType::Boolean;
    InputActionMap.Add(TEXT("IA_Bool_MouseLeft"),IA_Bool_MouseLeft);

	//IA_Bool_MouseRight用来处理鼠标右键的输入
	UInputAction* IA_Bool_MouseRight=CreateDefaultSubobject<UInputAction>(TEXT("IA_Bool_MouseRight"));
    IA_Bool_MouseRight->bConsumeInput=true;
    IA_Bool_MouseRight->bTriggerWhenPaused=false;
    IA_Bool_MouseRight->bReserveAllMappings=false;
    IA_Bool_MouseRight->ValueType=EInputActionValueType::Boolean;
    InputActionMap.Add(TEXT("IA_Bool_MouseRight"),IA_Bool_MouseRight);
	
	//IA_Float2D_Keyboard用来处理按WASD键的输入
	UInputAction* IA_Float2D_Keyboard=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float2D_Keyboard"));
	IA_Float2D_Keyboard->bConsumeInput=true;
	IA_Float2D_Keyboard->bTriggerWhenPaused=false;
	IA_Float2D_Keyboard->bReserveAllMappings=false;
	IA_Float2D_Keyboard->ValueType=EInputActionValueType::Axis2D;
	InputActionMap.Add(TEXT("IA_Float2D_Keyboard"),IA_Float2D_Keyboard);
	
	//IA_Float1D_Up用来处理按EQ键的输入
	UInputAction* IA_Float1D_Up=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float1D_Up"));
	IA_Float1D_Up->bConsumeInput=true;
	IA_Float1D_Up->bTriggerWhenPaused=false;
	IA_Float1D_Up->bReserveAllMappings=false;
	IA_Float1D_Up->ValueType=EInputActionValueType::Axis1D;
	InputActionMap.Add(TEXT("IA_Float1D_Up"),IA_Float1D_Up);
	
	//IA_Float1D_MouseWheel用来处理滚动鼠标中间的输入
	UInputAction* IA_Float1D_MouseWheel=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float1D_MouseWheel"));
	IA_Float1D_MouseWheel->bConsumeInput=true;
	IA_Float1D_MouseWheel->bTriggerWhenPaused=false;
	IA_Float1D_MouseWheel->bReserveAllMappings=false;
	IA_Float1D_MouseWheel->ValueType=EInputActionValueType::Axis1D;
	InputActionMap.Add(TEXT("IA_Float1D_MouseWheel"),IA_Float1D_MouseWheel);
	
	//Game_Input_Context 用来映射按键输入
	UInputMappingContext* Game_Input_Context=CreateDefaultSubobject<UInputMappingContext>(TEXT("Game_Input_Context"));
	InputMappingContextMap.Add(FName(TEXT("Game_Input_Context")),Game_Input_Context);
}

// Called when the game starts or when spawned
void ACommonBasePawn::BeginPlay()
{
	Super::BeginPlay();
	AddEnhancedContext();

	//确保只有一个Camera处于激活状态
	Ensure_Only_One_Camera_Active_Internal();
}

// Called every frame
void ACommonBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Tick调用自动旋转
	Pawn_Round_Event();
}

// Called to bind functionality to input
void ACommonBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
			//IA_Float2D_Mouse用来处理鼠标在XY轴上的输入
            	UInputAction* IA_Float2D_Mouse=NewObject<UInputAction>(this,TEXT("IA_Float2D_Mouse"));
            	IA_Float2D_Mouse->bConsumeInput=true;
            	IA_Float2D_Mouse->bTriggerWhenPaused=false;
            	IA_Float2D_Mouse->bReserveAllMappings=false;
            	IA_Float2D_Mouse->ValueType=EInputActionValueType::Axis2D;
            	InputActionMap.Add(TEXT("IA_Float2D_Mouse"),IA_Float2D_Mouse);
            
            	//IA_Bool_MouseLeft用来处理鼠标左键的输入
            	UInputAction* IA_Bool_MouseLeft=NewObject<UInputAction>(this,TEXT("IA_Bool_MouseLeft"));
                IA_Bool_MouseLeft->bConsumeInput=true;
                IA_Bool_MouseLeft->bTriggerWhenPaused=false;
                IA_Bool_MouseLeft->bReserveAllMappings=false;
                IA_Bool_MouseLeft->ValueType=EInputActionValueType::Boolean;
                InputActionMap.Add(TEXT("IA_Bool_MouseLeft"),IA_Bool_MouseLeft);
            
            	//IA_Bool_MouseRight用来处理鼠标右键的输入
            	UInputAction* IA_Bool_MouseRight=NewObject<UInputAction>(this,TEXT("IA_Bool_MouseRight"));
                IA_Bool_MouseRight->bConsumeInput=true;
                IA_Bool_MouseRight->bTriggerWhenPaused=false;
                IA_Bool_MouseRight->bReserveAllMappings=false;
                IA_Bool_MouseRight->ValueType=EInputActionValueType::Boolean;
                InputActionMap.Add(TEXT("IA_Bool_MouseRight"),IA_Bool_MouseRight);
            	
            	//IA_Float2D_Keyboard用来处理按WASD键的输入
            	UInputAction* IA_Float2D_Keyboard=NewObject<UInputAction>(this,TEXT("IA_Float2D_Keyboard"));
            	IA_Float2D_Keyboard->bConsumeInput=true;
            	IA_Float2D_Keyboard->bTriggerWhenPaused=false;
            	IA_Float2D_Keyboard->bReserveAllMappings=false;
            	IA_Float2D_Keyboard->ValueType=EInputActionValueType::Axis2D;
            	InputActionMap.Add(TEXT("IA_Float2D_Keyboard"),IA_Float2D_Keyboard);
            	
            	//IA_Float1D_Up用来处理按EQ键的输入
            	UInputAction* IA_Float1D_Up=NewObject<UInputAction>(this,TEXT("IA_Float1D_Up"));
            	IA_Float1D_Up->bConsumeInput=true;
            	IA_Float1D_Up->bTriggerWhenPaused=false;
            	IA_Float1D_Up->bReserveAllMappings=false;
            	IA_Float1D_Up->ValueType=EInputActionValueType::Axis1D;
            	InputActionMap.Add(TEXT("IA_Float1D_Up"),IA_Float1D_Up);
            	
            	//IA_Float1D_MouseWheel用来处理滚动鼠标中间的输入
            	UInputAction* IA_Float1D_MouseWheel=NewObject<UInputAction>(this,TEXT("IA_Float1D_MouseWheel"));
            	IA_Float1D_MouseWheel->bConsumeInput=true;
            	IA_Float1D_MouseWheel->bTriggerWhenPaused=false;
            	IA_Float1D_MouseWheel->bReserveAllMappings=false;
            	IA_Float1D_MouseWheel->ValueType=EInputActionValueType::Axis1D;
            	InputActionMap.Add(TEXT("IA_Float1D_MouseWheel"),IA_Float1D_MouseWheel);
            	
            	//Game_Input_Context 用来映射按键输入
            	UInputMappingContext* Game_Input_Context=NewObject<UInputMappingContext>(this,TEXT("Game_Input_Context"));
            	InputMappingContextMap.Add(FName(TEXT("Game_Input_Context")),Game_Input_Context);




























		
		//将IA_Float2D_Mouse与鼠标在XY轴上的输入绑定,Y轴需要其值作为FVector2D的Y,因为值不能都存在FVector2D的X.
		Game_Input_Context->MapKey(IA_Float2D_Mouse,EKeys::MouseX);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingMouseY= Game_Input_Context->MapKey(IA_Float2D_Mouse,EKeys::MouseY);
		FEnhancedActionKeyMappingMouseY.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Float2D_Mouse"),ETriggerEvent::Triggered,this,&ACommonBasePawn::MouseXYMove_Internal);

		//将IA_Bool_MouseLeft与鼠标左键的输入绑定
		Game_Input_Context->MapKey(IA_Bool_MouseLeft,EKeys::LeftMouseButton);
		EnhancedInputComponent->BindAction(IA_Bool_MouseLeft,ETriggerEvent::Started,this,&ACommonBasePawn::MouseLeft_Internal);
		EnhancedInputComponent->BindAction(IA_Bool_MouseLeft,ETriggerEvent::Completed,this,&ACommonBasePawn::MouseLeft_Internal);
		
		//将IA_Bool_MouseRight与鼠标右键的输入绑定
		Game_Input_Context->MapKey(IA_Bool_MouseRight,EKeys::RightMouseButton);
		EnhancedInputComponent->BindAction(IA_Bool_MouseRight,ETriggerEvent::Started,this,&ACommonBasePawn::MouseRight_Internal);
		EnhancedInputComponent->BindAction(IA_Bool_MouseRight,ETriggerEvent::Completed,this,&ACommonBasePawn::MouseRight_Internal);
		
		//将IA_Float2D_Keyboard与键盘WASD键的输入绑定,W键的值作为FVector2D的Y值。S键的值作为FVector2D的Y值,需要反向。A键的值作为FVector2D的X,需要反向。D键的值作为FVector2D的X。因为值不能都存在FVector2D的X上.
		Game_Input_Context->MapKey(IA_Float2D_Keyboard,EKeys::W);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardS = Game_Input_Context->MapKey(IA_Float2D_Keyboard,EKeys::S);
		FEnhancedActionKeyMappingKeyboardS.Modifiers.Add(NewObject<UInputModifierNegate>());
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardA = Game_Input_Context->MapKey(IA_Float2D_Keyboard,EKeys::A);
		FEnhancedActionKeyMappingKeyboardA.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		FEnhancedActionKeyMappingKeyboardA.Modifiers.Add(NewObject<UInputModifierNegate>());
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardD = Game_Input_Context->MapKey(IA_Float2D_Keyboard,EKeys::D);
		FEnhancedActionKeyMappingKeyboardD.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		EnhancedInputComponent->BindAction(IA_Float2D_Keyboard,ETriggerEvent::Triggered,this,&ACommonBasePawn::KeyboardWASDMove_Internal);
		
		//将IA_Float1D_Up与键盘EQ键的输入绑定,E键的值为正,Q键的值为负.
		Game_Input_Context->MapKey(IA_Float1D_Up,EKeys::E);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardQ=Game_Input_Context->MapKey(IA_Float1D_Up,EKeys::Q);
		FEnhancedActionKeyMappingKeyboardQ.Modifiers.Add(NewObject<UInputModifierNegate>());
		EnhancedInputComponent->BindAction(IA_Float1D_Up,ETriggerEvent::Triggered,this,&ACommonBasePawn::KeyboardEQUp_Internal);
		
		//将IA_Float1D_MouseWheel与鼠标滚动的输入绑定
		Game_Input_Context->MapKey(IA_Float1D_MouseWheel,EKeys::MouseWheelAxis);
		EnhancedInputComponent->BindAction(IA_Float1D_MouseWheel,ETriggerEvent::Triggered,this,&ACommonBasePawn::MouseWheel_Internal);
	}
}




void ACommonBasePawn::AddEnhancedContext()
{
	if (APlayerController* PlayerController=Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContextMap.FindChecked("Game_Input_Context"),0);
		}
	}
}


















#pragma region 输入事件输入相关


void ACommonBasePawn::MouseXYMove_Internal(const FInputActionInstance& Value)
{
	MouseXYMoveEvent(Value.GetValue().Get<FVector2D>());
}

void ACommonBasePawn::KeyboardWASDMove_Internal(const FInputActionInstance& Value)
{
	KeyboardWASDMoveEvent(Value.GetValue().Get<FVector2D>());
}

void ACommonBasePawn::KeyboardEQUp_Internal(const FInputActionInstance& Value)
{
	KeyboardEQUpEvent(Value.GetValue().Get<float>());
}

void ACommonBasePawn::MouseWheel_Internal(const FInputActionInstance& Value)
{
	MouseWheelEvent(Value.GetValue().Get<float>());
}

void ACommonBasePawn::MouseLeft_Internal(const FInputActionInstance& Value)
{
	MouseLeftButtonEvent(Value.GetValue().Get<bool>());
}

void ACommonBasePawn::MouseRight_Internal(const FInputActionInstance& Value)
{
	MouseRightButtonEvent(Value.GetValue().Get<bool>());
}




void ACommonBasePawn::MouseXYMoveEvent_Implementation(const FVector2D Value)
{
	//当bCan_Move为false时退出.
	if (!bCan_Move) return;

	//当前正在播放漫游Sequence动画是退出
	if (bPlayingSequence) return;

	//当前正在进行视口混合时退出
	if (bPlayingViewBlend) return;
	
	//Normal_Camera相机和SpringArm_Camera相机都激活时
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		OnAll_Camera_Active_MouseXYMoveEvent_Internal(Value);
		return;
	}
	
	//当Normal_Camera相机激活,且SpringArm_Camera相机未激活时调用
	if (Normal_Camera->IsActive()&&!SpringArm_Camera->IsActive())
	{
		OnOnlyNormal_Camera_Active_MouseXYMoveEvent_Internal(Value);
		return;
	}

	//当SpringArm_Camera相机激活,且Normal_Camera相机未激活时调用
	if (SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		OnOnlySpringArm_Camera_Active_MouseXYMoveEvent_Internal(Value);
		return;
	}

	//Normal_Camera相机和SpringArm_Camera相机都未激活时
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		OnNo_Camera_Active_MouseXYMoveEvent_Internal(Value);
	}
}
void ACommonBasePawn::KeyboardWASDMoveEvent_Implementation(const FVector2D Value)
{
}
void ACommonBasePawn::KeyboardEQUpEvent_Implementation(const float Value)
{
}

void ACommonBasePawn::MouseWheelEvent_Implementation(const float Value)
{
	//检测是否可移动
	if (!bCan_Move) return;

	//当前正在播放漫游Sequence动画是退出
	if (bPlayingSequence) return;

	//当前正在进行视口混合时退出
	if (bPlayingViewBlend) return;
	
	//检测当前角度是否在限定范围内
	if (GetActorRotation().Pitch<SpringArm_Camera_Rotate_Pitch_Range.X||GetActorRotation().Pitch>SpringArm_Camera_Rotate_Pitch_Range.Y)
	{
		//不在限定范围内
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("角度Pitch超出范围，无法缩放视角"));
		UE_LOG(LogTemp,Warning,TEXT("角度Pitch超出范围，无法缩放视角"));
		return;
	}

	if (!Normal_Camera_To_SpringArm_Camera())
	{
		return;
	}

	//弹簧臂的变化值
	const float SpringArmChange_Value=SpringArm->TargetArmLength*(0.2f)>1000.f?SpringArm->TargetArmLength*(0.2f)*Value:1000.f*Value;
	
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
		FOnTimelineEvent OnTimelineEvent;
		OnTimelineEvent.BindDynamic(this,&ACommonBasePawn::SpringArm_TimeLine_Update);
		SpringArm_TimeLine->SetTimelinePostUpdateFunc(OnTimelineEvent);
	}
	SpringArm_TimeLine->PlayFromStart();
}

void ACommonBasePawn::MouseLeftButtonEvent_Implementation(const bool Value)
{
	bL_Mouse_Pressed=Value;
}

void ACommonBasePawn::MouseRightButtonEvent_Implementation(const bool Value)
{
	bR_Mouse_Pressed=Value;
}


#pragma endregion

#pragma region 处理Pawn移动相关

void ACommonBasePawn::OnOnlySpringArm_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value)
{
	//检测当前角度是否在限定范围内
	if (!(UKismetMathLibrary::InRange_FloatFloat(GetActorRotation().Pitch,SpringArm_Camera_Rotate_Pitch_Range.X,SpringArm_Camera_Rotate_Pitch_Range.Y),true,true))
	{
		//不在范围内
		SpringArm_Camera_To_Normal_Camera();

		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
		
		return;
	}

	//鼠标左键和右键都未按下.
	if (bL_Mouse_Pressed!=true&&bR_Mouse_Pressed!=true)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("鼠标左键和鼠标右键都未按下,暂无具体执行"));
		UE_LOG(LogTemp,Warning,TEXT("鼠标左键和鼠标右键都未按下,暂无具体执行"));
	}

	
	//仅鼠标左键按下
	if (bL_Mouse_Pressed==true&&bR_Mouse_Pressed!=true)
	{
		SpringArm_Camera_Move_Pawn(Value);

		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}

	//仅鼠标右键按下
	if (bL_Mouse_Pressed!=true&&bR_Mouse_Pressed==true)
	{
		SpringArm_Camera_Rotate_Pawn(Value);
		
		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}

	//鼠标左键和鼠标右键都已按下
	if (bL_Mouse_Pressed==true&&bR_Mouse_Pressed==true)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
		UE_LOG(LogTemp,Warning,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
	}
}

void ACommonBasePawn::OnOnlyNormal_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value)
{
	//仅鼠标左键按下
	if (bL_Mouse_Pressed==true&&bR_Mouse_Pressed!=true)
	{
		
		//if (GetActorRotation().Pitch>=View_Rotate_Pitch_Range.X&&GetActorRotation().Pitch<=View_Rotate_Pitch_Range.Y)
		
		if (UKismetMathLibrary::InRange_FloatFloat(this->GetActorRotation().Pitch,SpringArm_Camera_Rotate_Pitch_Range.X,SpringArm_Camera_Rotate_Pitch_Range.Y,true,true))
		{
			
			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,FString::SanitizeFloat(this->GetActorRotation().Pitch));
			//角度在范围内,转为SpringArm_Camera
			if (Normal_Camera_To_SpringArm_Camera())
			{
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera转换SpringArm_Camera成功"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera转换SpringArm_Camera失败"));
			}

			
			
			//重置自动旋转时间
			Reset_Auto_Rotate_Total_Time();
		}
		else
		{
			//角度不在范围内,继续使用Normal_Camera的移动
			Normal_Camera_Move_Pawn(Value);

			GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera_移动"));
			
			//重置自动旋转时间
			Reset_Auto_Rotate_Total_Time();
		}
	}

	//仅鼠标右键按下
	if (bL_Mouse_Pressed!=true&&bR_Mouse_Pressed==true)
	{
		//进行旋转操作
		Normal_Camera_Rotate_Pawn(Value);
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Normal_Camera_旋转"));
		
		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}

	//鼠标左键和鼠标右键都已按下
	if (bL_Mouse_Pressed==true&&bR_Mouse_Pressed==true)
	{
		GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
		UE_LOG(LogTemp,Warning,TEXT("鼠标左键和鼠标右键都已按下,暂无具体执行"));
	}
	
	
}

void ACommonBasePawn::OnNo_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value)
{
	UE_LOG(LogTemp,Warning,TEXT("Erro：Pawn上没有激活的Camera"));
}

void ACommonBasePawn::OnAll_Camera_Active_MouseXYMoveEvent_Internal(const FVector2D& Value)
{
	GEngine->AddOnScreenDebugMessage(-1,3.f,FColor::White,TEXT("Erro：Pawn的Camera都已激活,这将出现逻辑错误"));
	UE_LOG(LogTemp,Warning,TEXT("Erro：Pawn的Camera都已激活,这将出现逻辑错误"));
}

void ACommonBasePawn::Ensure_Only_One_Camera_Active_Internal()
{
	
	if (SpringArm_Camera->IsActive())
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
	if (bL_Mouse_Pressed&&!bR_Mouse_Pressed)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bR_Mouse_Pressed&&!bL_Mouse_Pressed)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bL_Mouse_Pressed&&bR_Mouse_Pressed)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::OnOnlyMouseRightButtonPressed_Internal(const FVector2D& Value)
{
	//仅当鼠标左键按下,且鼠标右键未按下时.
	if (bL_Mouse_Pressed&&!bR_Mouse_Pressed)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bR_Mouse_Pressed&&!bL_Mouse_Pressed)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bL_Mouse_Pressed&&bR_Mouse_Pressed)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::OnMouseAllButtonPressed_Internal(const FVector2D& Value)
{
	//仅当鼠标左键按下,且鼠标右键未按下时.
	if (bL_Mouse_Pressed&&!bR_Mouse_Pressed)
	{
		OnOnlyMouseLeftButtonPressed_Internal(Value);
		return;
	}
	
	//仅当鼠标右键按下,且鼠标左键未按下时.
	if (bR_Mouse_Pressed&&!bL_Mouse_Pressed)
	{
		OnOnlyMouseRightButtonPressed_Internal(Value);
		return;
	}
	
	//当鼠标左键与右键都按下时调用
	if (bL_Mouse_Pressed&&bR_Mouse_Pressed)
	{
		OnMouseAllButtonPressed_Internal(Value);
	}
}

void ACommonBasePawn::SpringArm_TimeLine_Update()
{
	const float SpringArm_Final_Length= UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength,TargetSpringArm_Length,UGameplayStatics::GetWorldDeltaSeconds(this),2.f);
	SpringArm->TargetArmLength=SpringArm_Final_Length;
}




void ACommonBasePawn::SpringArm_Camera_Move_Pawn_Implementation(const FVector2D& Value)
{
	//根据弹簧臂长度计算移动的值
	const float MoveValue=this->SpringArm->TargetArmLength*(0.008)>=50?this->SpringArm->TargetArmLength*(0.008):50;

	const APlayerCameraManager* PlayerCameraManager= UGameplayStatics::GetPlayerCameraManager(this,0);

	//计算在X方向上的移动距离.
	const FVector XVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetRightVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector XVector=XVectorNormal*(MoveValue*Value.X*-1.f);
	
	//计算在Y方向上的移动距离.
	const FVector YVectorNormal=UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(FRotator(
		0,
		PlayerCameraManager->GetCameraRotation().Yaw,
		PlayerCameraManager->GetCameraRotation().Roll)));
	const FVector YVector=YVectorNormal*(MoveValue*Value.Y*-1.f);

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

void ACommonBasePawn::SpringArm_Camera_Rotate_Pawn_Implementation(const FVector2D& Value)
{
	//计算旋转的Y:Pitch值
	const float New_TemPitchValue=Value.Y*(1.f)+GetActorRotation().Pitch;
	const float NewPitchValue=New_TemPitchValue>=SpringArm_Camera_Rotate_Pitch_Range.X?(New_TemPitchValue<=SpringArm_Camera_Rotate_Pitch_Range.Y?New_TemPitchValue:SpringArm_Camera_Rotate_Pitch_Range.Y):SpringArm_Camera_Rotate_Pitch_Range.X;

	//计算旋转的Z:Yaw值
	const float NewYawValue=Value.X*(1.f)+GetActorRotation().Yaw;

	//更新Pawn的角度.
	SetActorRotation(FRotator(
		NewPitchValue,
		NewYawValue,
		GetActorRotation().Roll));
}

void ACommonBasePawn::Normal_Camera_Rotate_Pawn_Implementation(const FVector2D& Value)
{
	const FRotator CurrentRotation=GetActorRotation();
	
	//计算旋转的Y:Pitch值,限定Pitch的角度,因为Pitch的值为-90或90度时会有问题.
	const float New_Tem_Pitch_Value=Value.Y*(Normal_Camera_Rotate_Param.Y)+CurrentRotation.Pitch;
	const float NewPitchValue=New_Tem_Pitch_Value>=Normal_Camera_Rotate_Pitch_Range.X?(New_Tem_Pitch_Value<=Normal_Camera_Rotate_Pitch_Range.Y?New_Tem_Pitch_Value:Normal_Camera_Rotate_Pitch_Range.Y):Normal_Camera_Rotate_Pitch_Range.X;
	
	//计算旋转的Z:Yaw值
	const float NewYawValue=Value.X*(Normal_Camera_Rotate_Param.X)+CurrentRotation.Yaw;
	
	//更新Pawn的角度.
	SetActorRotation(FRotator(NewPitchValue,NewYawValue,CurrentRotation.Roll));
}

void ACommonBasePawn::Normal_Camera_Move_Pawn_Implementation(const FVector2D& Value)
{
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

bool ACommonBasePawn::Normal_Camera_To_SpringArm_Camera_Implementation()
{
	if (SpringArm_Camera->IsActive())
	{
		//当弹簧臂相机激活时
		if (!SpringArm->bEnableCameraLag)
		{
			SpringArm->bEnableCameraLag=true;
		}
		return true;
	}
	
	const FRotator CurrentCameraRotate=GetActorRotation();	//获取当前的方向(此时方向应该与Norma_Camera一致)
	const FVector CurrentCameraLoc=GetActorLocation();		//获取当前的位置(此时位置应该与Norma_Camera一致)

	//检查当前角度的Pitch值，是否处于限定范围内
	if (CurrentCameraRotate.Pitch<SpringArm_Camera_Rotate_Pitch_Range.X||CurrentCameraRotate.Pitch>SpringArm_Camera_Rotate_Pitch_Range.Y)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Erro:Z轴位置错误,超出限定范围,无法切换相机"));
		UE_LOG(LogTemp,Warning,TEXT("Erro:Z轴位置错误,超出限定范围,无法切换相机"));
		return false;
	}
	
	//检查当前位置的Z轴高度，是否处于限定范围
	if (CurrentCameraLoc.Z<Pawn_Min_Z_Value)				
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Erro:相机角度出现错误,超过限定范围,无法切换相机"));
		UE_LOG(LogTemp,Warning,TEXT("Erro:相机角度出现错误,超过限定范围,无法切换相机"))
		return false;
	}

	//计算最终Pawn的位置，并计算弹簧臂应该设定的长度
	const float Camera_Vertical_Value=CurrentCameraLoc.Z-Pawn_Min_Z_Value;						//计算相机的垂直方向长度
	const double DegreeValue=FMath::Abs(UKismetMathLibrary::DegSin(CurrentCameraRotate.Pitch));		//代入最终弹簧臂相机的角度，使用sin函数
	const float FinalSpringArmLength=Camera_Vertical_Value/DegreeValue;							//最终的斜边长度（弹簧臂长度）

	//更新设置弹簧臂的长度
	this->SpringArm->TargetArmLength=FinalSpringArmLength;

	//计算Pawn最终的位置
	const FVector ForwardVector=UKismetMathLibrary::Normal(UKismetMathLibrary::GetForwardVector(CurrentCameraRotate));
	const FVector FinalLocation=FinalSpringArmLength*ForwardVector+CurrentCameraLoc;

	//更新Pawn的位置
	SetActorLocation(FinalLocation);

	//位置都已计算完毕,开始切换相机.
	SpringArm_Camera->SetActive(true);	//激活弹簧臂相机
	Normal_Camera->SetActive(false);	//灭活弹簧臂相机

	return true;
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

void ACommonBasePawn::Pawn_Round_Event_Implementation()
{
	//检测是否可移动.
	if (!bCan_Move)
	{
		No_Action_Total_Time=0.f;
		return;
	}

	//检测当前是否正在播放漫游动画
	if (bPlayingSequence)
	{
		No_Action_Total_Time=0.f;
		return;
	}

	//检测当前是否正在进行视角混合
	if (bPlayingViewBlend)
	{
		No_Action_Total_Time=0.f;
		return;
	}
	
	//监测未操作时间是否大于设定值
	if (No_Action_Total_Time<Start_Auto_Rotate_Time)
	{
		No_Action_Total_Time+=UGameplayStatics::GetWorldDeltaSeconds(this);
		return;
	}

	//先进行转换,绕点旋转必须使用SpringArm_Camera
	if (!Normal_Camera_To_SpringArm_Camera())
	{
		return;
	}
	
	//每帧的转动速度
	const float PerTick_Auto_Rotate_Speed=Auto_Rotate_Speed*UGameplayStatics::GetWorldDeltaSeconds(this);

	//旋转,更新角度
	SpringArm_Camera_Rotate_Pawn(FVector2D(PerTick_Auto_Rotate_Speed,0));
}

void ACommonBasePawn::Reset_Auto_Rotate_Total_Time_Implementation()
{
	No_Action_Total_Time=0.f;
}

void ACommonBasePawn::Set_Camera_View_By_Actor_Implementation(const FVector& InVector, const FRotator& InRotator)
{
	//确保切换到Normal_Camera
	if (!SpringArm_Camera_To_Normal_Camera())	return;

	//更新位置和角度
	SetActorLocation(InVector);
	SetActorRotation(InRotator);
}

void ACommonBasePawn::Blend_View_Event_Implementation(const FVector& InVector, const FRotator& InRotator,const float BlendTime,EViewTargetBlendFunction BlendFunc, float BlendExp, bool bLockOutgoing)
{
	if (!View_Blend_Tem_Actor)
	{
		ACameraActor* TemCameraActor=GetWorld()->SpawnActor<ACameraActor>(InVector,InRotator);
		TemCameraActor->GetCameraComponent()->SetConstraintAspectRatio(false);
		View_Blend_Tem_Actor=TemCameraActor;
	}
	else
	{
		View_Blend_Tem_Actor->SetActorLocation(InVector);
		View_Blend_Tem_Actor->SetActorRotation(InRotator);
	}

	if (APlayerController* PlayerController=Cast<APlayerController>(GetController()))
	{
		//进行混合
		PlayerController->SetViewTargetWithBlend(View_Blend_Tem_Actor,BlendTime,BlendFunc,BlendExp,bLockOutgoing);

		//用于定义当前处于视口混合状态
		bPlayingViewBlend=true;
		
		//设置相机视口应处于的位置.
		Set_Camera_View_By_Actor(InVector,InRotator);

		//重置自动旋转时间
		Reset_Auto_Rotate_Total_Time();
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.f,FColor::White,TEXT("Erro:视角混合错误名,无法获取APlayerController"));
		UE_LOG(LogTemp,Warning,TEXT("Erro:视角混合错误名,无法获取APlayerController"))
	}
	
}

#pragma endregion
