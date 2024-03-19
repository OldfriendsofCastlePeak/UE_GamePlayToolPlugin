// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleTouchComponent.h"

#include "EnhancedInputSubsystems.h"


// Sets default values for this component's properties
USingleTouchComponent::USingleTouchComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USingleTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USingleTouchComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USingleTouchComponent::RegisterInputMapping()
{
	//检测Touch_Input_Mapping_Context是否有效,防止多次创建
	if (!Touch_Input_Mapping_Context) Touch_Input_Mapping_Context=NewObject<UInputMappingContext>(this,TEXT("MouseInputMappingContextMap"));

	//检测数组是否有UInputAction,防止多次创建
	if (Touch_Input_Action_Map.Num()==0)
	{
		UInputAction* Tem_IA_Float2D_Touch=NewObject<UInputAction>(this,TEXT("IA_Float2D_Touch1"));
		Tem_IA_Float2D_Touch->bConsumeInput=true;
		Tem_IA_Float2D_Touch->bTriggerWhenPaused=false;
		Tem_IA_Float2D_Touch->bReserveAllMappings=false;
    			
		//设置值的类型为Axis3D,这样其Y值将可以用来进行额外的判断
		Tem_IA_Float2D_Touch->ValueType=EInputActionValueType::Axis3D;

		//将UInputAction对象添加到TMap容器中
		Touch_Input_Action_Map.Add(TEXT("IA_Float2D_Touch1"),Tem_IA_Float2D_Touch);

		//添加映射
		FEnhancedActionKeyMapping& EnhancedActionKeyMapping= Touch_Input_Mapping_Context->MapKey(Tem_IA_Float2D_Touch,EKeys::TouchKeys[0]);
	}
	
	//获取InputComponent:只要是Actor就有UInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
    {
		//获取APlayerController:增强输入需要使用AddMappingContext方法来添加映射
    	if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
    	{
    		if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    		{
    			//先检查一下,防止重复注册
    			const TArray<TUniquePtr<FEnhancedInputActionEventBinding>>* ActionEventBindings= &EnhancedInputComponent->GetActionEventBindings();
    			bool bHasRegister=false;
    			for (int i=0;i<ActionEventBindings->Num();i++)
               {
	               if (EnhancedInputComponent->GetActionEventBindings()[i].Get()->GetAction()==Touch_Input_Action_Map.FindChecked(TEXT("IA_Float2D_Touch1")))
	               {
	               		bHasRegister=true;
	               		return;
	               }
               }

			    if (bHasRegister) return;
    			
    			//添加触碰的Pressed、Moved、Released的委托绑定
    			EnhancedInputComponent->BindAction(Touch_Input_Action_Map.FindChecked(TEXT("IA_Float2D_Touch1")),ETriggerEvent::Started,this,&UTouchBaseComponent::Touch_Released_Internal_Event);
    			EnhancedInputComponent->BindAction(Touch_Input_Action_Map.FindChecked(TEXT("IA_Float2D_Touch1")),ETriggerEvent::Triggered,this,&UTouchBaseComponent::Touch_Moved_Internal_Event);
    			EnhancedInputComponent->BindAction(Touch_Input_Action_Map.FindChecked(TEXT("IA_Float2D_Touch1")),ETriggerEvent::Completed,this,&UTouchBaseComponent::Touch_Released_Internal_Event);

    			//注册
    			EnhancedInputLocalPlayerSubsystem->AddMappingContext(Touch_Input_Mapping_Context,InputPriority);
    			
    		}
    	}
    }
}

void USingleTouchComponent::UnRegisterInputMapping()
{
	if (Touch_Input_Mapping_Context)
	{
		if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(Touch_Input_Mapping_Context);
				bAuto_Register_Input_Mapping=false;
			}
		}
	}
}


void USingleTouchComponent::Touch_Pressed_Internal_Event(const FInputActionInstance& Value)
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 1;

	//设置数组容器对应元素的状态
	Touch_State_Index[0]=1;

	//设置初始按下位置为最后触碰位置
	Last_Touch_Position=Location;
	
	//触发委托
	On_Touch_Pressed.Broadcast(Location);
	
}

void USingleTouchComponent::Touch_Moved_Internal_Event(const FInputActionInstance& Value)
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 2;

	//设置数组容器对应元素的状态
	Touch_State_Index[0]=2;

	
	if (Last_Touch_Position==FVector(-1,-1,-1))
	{
		//更新最后触碰位置
		Last_Touch_Position=Location;
	}
	else
	{
		//计算移动向量,使用当前位置-上一次位置
        const FVector MoveVector=Location-Last_Touch_Position;

		//更新最后触碰位置
		Last_Touch_Position=Location;

		//触发委托
		On_Touch_Moved.Broadcast(MoveVector);
	}
}

void USingleTouchComponent::Touch_Released_Internal_Event(const FInputActionInstance& Value) 
{
	FVector Location = Value.GetValue().Get<FVector>();
	
	//设置按下的触碰位置Z为1,移动的Z值为2,松开为0.
	Location.Z = 0;

	//设置数组容器对应元素的状态
	Touch_State_Index[0]=0;

	//设置初始按下位置为最后触碰位置
	Last_Touch_Position=FVector(-1,-1,-1);
	
	//触发委托
	On_Touch_Released.Broadcast(Location);
}

