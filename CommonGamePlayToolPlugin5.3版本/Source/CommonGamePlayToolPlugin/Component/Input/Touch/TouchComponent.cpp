// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "../../../InputModifier/InputModifierAddVector.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// Sets default values for this component's properties
UTouchComponent::UTouchComponent()
{
	//Tick没必要开启
	PrimaryComponentTick.bCanEverTick = false;

	//设置容器的最大索引数
	Touch_State_Index.SetNum(10);
}

// Called when the game starts
void UTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	//注册触碰输入映射
	RegisterTouchInput();
}

// Called every frame
void UTouchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UTouchComponent::RegisterTouchInput()
{
	//获取输入组件:只有是Actor就有UInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
    	{
			//获取APlayerController:增强输入需要使用AddMappingContext方法来添加映射
    		if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
    		{
    			//Touch_Input_Mapping_Context用来映射触碰输入
    			Touch_Input_Mapping_Context=NewObject<UInputMappingContext>(this,TEXT("MouseInputMappingContextMap"));
    			
    			//创建10个触碰的UInputAction
    			for (int i=0;i<10;i++)
    			{
    				//设置每个UInputAction对象的名字:IA_Float2D_Touch+[i]
    				FString UInputActionName="IA_Float2D_Touch"+FString::FromInt(i);
    				UInputAction* Tem_IA_Float2D_Touch=NewObject<UInputAction>(this,*UInputActionName);
    				
    				Tem_IA_Float2D_Touch->bConsumeInput=true;
    				Tem_IA_Float2D_Touch->bTriggerWhenPaused=false;
    				Tem_IA_Float2D_Touch->bReserveAllMappings=false;
    				
    				//设置值的类型为Axis3D,这样其Y值将可以用来进行额外的判断
    				Tem_IA_Float2D_Touch->ValueType=EInputActionValueType::Axis3D;

    				//将UInputAction对象添加到TMap容器中
    				Touch_Input_Action_Map.Add(*UInputActionName,Tem_IA_Float2D_Touch);

    				//添加映射
    				FEnhancedActionKeyMapping& EnhancedActionKeyMapping= Touch_Input_Mapping_Context->MapKey(Tem_IA_Float2D_Touch,EKeys::TouchKeys[i]);
    				UInputModifierAddVector* Tem_InputModifierAddVector= NewObject<UInputModifierAddVector>();

    				//设置不同的触碰UInputAction的Z值,例如:触碰1需要加的Z值为0,触碰2需要加的Z值为1,以此类推,触碰10需要加的Z值为9
    				Tem_InputModifierAddVector->Vector=FVector(0,0,i-1>=0?i:0);
    				EnhancedActionKeyMapping.Modifiers.Add(Tem_InputModifierAddVector);

    				//添加触碰的Pressed、Moved、Released的委托绑定
    				EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Started,this,&UTouchComponent::Touch_Released_Internal_Event);
    				EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Triggered,this,&UTouchComponent::Touch_Moved_Internal_Event);
    				EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Completed,this,&UTouchComponent::Touch_Released_Internal_Event);
    			}
    			
    			//添加到上下文中
    			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    			{
    				EnhancedInputLocalPlayerSubsystem->AddMappingContext(Touch_Input_Mapping_Context,0);
    			}
    		}
    	}
}

void UTouchComponent::UnRegisterTouchInput()
{
	if (Touch_Input_Mapping_Context)
	{
		if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(Touch_Input_Mapping_Context);
			}
		}
	}
}

void UTouchComponent::Touch_Pressed_Internal_Event(const FInputActionInstance& Value)
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//将FVector的Y用作触碰的指头索引
	const uint8 FingerIndex = Location.Z;
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 1;

	//设置数组容器对应元素的状态
	Touch_State_Index[FingerIndex]=1;
	
	//触发委托
	On_Touch_Pressed_TwoParam.Broadcast(Location,FingerIndex);
}

void UTouchComponent::Touch_Moved_Internal_Event(const FInputActionInstance& Value)
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//将FVector的Y用作触碰的指头索引
	const uint8 FingerIndex = Location.Z;
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 2;

	//设置数组容器对应元素的状态
	Touch_State_Index[FingerIndex]=2;
	
	//触发委托
	On_Touch_Moved_TwoParam.Broadcast(Location,FingerIndex);
}

void UTouchComponent::Touch_Released_Internal_Event(const FInputActionInstance& Value)
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//将FVector的Y用作触碰的指头索引
	const uint8 FingerIndex = Location.Z;
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 0;

	//设置数组容器对应元素的状态
	Touch_State_Index[FingerIndex]=0;

	












	
	
	//触发委托
	On_Touch_Released_TwoParam.Broadcast(Location,FingerIndex);
}

TArray<uint8> UTouchComponent::Get_Should_Released_Touch_Input_Array()
{
	//代表(0,0)的FVector2D
	FVector2D Vector(FVector::ZeroVector);

	//获取屏幕视口大小
	const FVector2D ViewportSizeVector = UWidgetLayoutLibrary::GetViewportSize(GetWorld());

	
	bool bIsCurrentlyPressed;

	//需要释放的触碰输入索引
	TArray<uint8> TouchIndexs;

	if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
	{
		for (size_t i = 0; i < Touch_State_Index.Num(); i++)
		{
			//获取对应的触碰输入状态
			PlayerController->GetInputTouchState(static_cast<ETouchIndex::Type>(i), Vector.X, Vector.Y, bIsCurrentlyPressed);
			
			//获取数组中的输入状态不为0,但是当前未处于按下状态的输入元素
			if (Touch_State_Index[i] && bIsCurrentlyPressed == false) TouchIndexs.Add(i);

			//获取触碰输入位置在当前视口之外的元素
			if ((
				Vector.X >= 0 &&
				Vector.X <= ViewportSizeVector.X &&
				Vector.Y >= 0 &&
				Vector.Y <= ViewportSizeVector.Y)
				==false)
			{
				TouchIndexs.Add(i);
			}
		}
	}
	return TouchIndexs;
}





