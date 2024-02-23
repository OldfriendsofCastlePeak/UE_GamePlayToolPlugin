// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CommonBasePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

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

	//创建用于第一人称世界形式的移动的相机.
	Normal_Camera=CreateDefaultSubobject<UCameraComponent>("Normal_Camera");
	Normal_Camera->SetupAttachment(Scene);







	
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
	InputMappingContextMap.Add(TEXT("Game_Input_Context"),Game_Input_Context);
}

// Called when the game starts or when spawned
void ACommonBasePawn::BeginPlay()
{
	Super::BeginPlay();
	AddEnhancedContext();
}

// Called every frame
void ACommonBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACommonBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//将IA_Float2D_Mouse与鼠标在XY轴上的输入绑定,Y轴需要其值作为FVector2D的Y,因为值不能都存在FVector2D的X.
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Mouse"),EKeys::MouseX);
		
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingMouseY= InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Mouse"),EKeys::MouseY);
		FEnhancedActionKeyMappingMouseY.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Float2D_Mouse"),ETriggerEvent::Triggered,this,&ACommonBasePawn::MouseXYMove_Internal);

		//将IA_Bool_MouseLeft与鼠标左键的输入绑定
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Bool_MouseLeft"),EKeys::LeftMouseButton);
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Bool_MouseLeft"),ETriggerEvent::Started,this,&ACommonBasePawn::MouseLeft_Internal);
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Bool_MouseLeft"),ETriggerEvent::Completed,this,&ACommonBasePawn::MouseLeft_Internal);
		
		//将IA_Bool_MouseRight与鼠标右键的输入绑定
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Bool_MouseRight"),EKeys::RightMouseButton);
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Bool_MouseRight"),ETriggerEvent::Started,this,&ACommonBasePawn::MouseRight_Internal);
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Bool_MouseRight"),ETriggerEvent::Completed,this,&ACommonBasePawn::MouseRight_Internal);
		
		//将IA_Float2D_Keyboard与键盘WASD键的输入绑定,W键的值作为FVector2D的Y值。S键的值作为FVector2D的Y值,需要反向。A键的值作为FVector2D的X,需要反向。D键的值作为FVector2D的X。因为值不能都存在FVector2D的X上.
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Keyboard"),EKeys::W);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardS = InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Keyboard"),EKeys::S);
		FEnhancedActionKeyMappingKeyboardS.Modifiers.Add(NewObject<UInputModifierNegate>());
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardA = InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Keyboard"),EKeys::A);
		FEnhancedActionKeyMappingKeyboardA.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		FEnhancedActionKeyMappingKeyboardA.Modifiers.Add(NewObject<UInputModifierNegate>());
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardD = InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float2D_Keyboard"),EKeys::D);
		FEnhancedActionKeyMappingKeyboardD.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Float2D_Mouse"),ETriggerEvent::Triggered,this,&ACommonBasePawn::KeyboardWASDMove_Internal);
		
		//将IA_Float1D_Up与键盘EQ键的输入绑定,E键的值为正,Q键的值为负.
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float1D_Up"),EKeys::E);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingKeyboardQ=InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float1D_Up"),EKeys::Q);
		FEnhancedActionKeyMappingKeyboardQ.Modifiers.Add(NewObject<UInputModifierNegate>());
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Float2D_Mouse"),ETriggerEvent::Triggered,this,&ACommonBasePawn::KeyboardEQUp_Internal);
		
		//将IA_Float1D_MouseWheel与鼠标滚动的输入绑定
		InputMappingContextMap.FindChecked("Game_Input_Context")->MapKey(InputActionMap.FindChecked("IA_Float1D_MouseWheel"),EKeys::MouseWheelAxis);
		EnhancedInputComponent->BindAction(InputActionMap.FindChecked("IA_Float2D_Mouse"),ETriggerEvent::Triggered,this,&ACommonBasePawn::MouseWheel_Internal);
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

	//Normal_Camera相机和SpringArm_Camera相机都激活时
	if (SpringArm_Camera->IsActive()&&Normal_Camera->IsActive())
	{
		OnAll_Camera_Active_Internal(Value);
		return;
	}
	
	//当Normal_Camera相机激活,且SpringArm_Camera相机未激活时调用
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		OnOnlyNormal_Camera_Active_Internal(Value);
		return;
	}

	//当SpringArm_Camera相机激活,且Normal_Camera相机未激活时调用
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		OnOnlySpringArm_Camera_Active_Internal(Value);
		return;
	}

	//Normal_Camera相机和SpringArm_Camera相机都未激活时
	if (!SpringArm_Camera->IsActive()&&!Normal_Camera->IsActive())
	{
		OnNo_Camera_Active_Internal(Value);
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

void ACommonBasePawn::OnOnlySpringArm_Camera_Active_Internal(const FVector2D& Value)
{
}

void ACommonBasePawn::OnOnlyNormal_Camera_Active_Internal(const FVector2D& Value)
{
}

void ACommonBasePawn::OnNo_Camera_Active_Internal(const FVector2D& Value)
{
	
}

void ACommonBasePawn::OnAll_Camera_Active_Internal(const FVector2D& Value)
{
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



#pragma endregion
