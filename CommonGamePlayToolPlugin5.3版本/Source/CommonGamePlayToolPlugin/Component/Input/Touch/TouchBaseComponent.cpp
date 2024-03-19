// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchBaseComponent.h"

#include "EnhancedInputSubsystems.h"


// Sets default values for this component's properties
UTouchBaseComponent::UTouchBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	//设置容器的最大索引数为1
	Touch_State_Index.SetNum(1);
	
}


// Called when the game starts
void UTouchBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//注册触碰输入映射
	//RegisterTouchInput();
}


// Called every frame
void UTouchBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTouchBaseComponent::RegisterInputMapping()
{
	//获取输入组件:只有是Actor就有UInputComponent
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
    	{
			//获取APlayerController:增强输入需要使用AddMappingContext方法来添加映射
    		if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
    		{
    			//Touch_Input_Mapping_Context用来映射触碰输入
    			Touch_Input_Mapping_Context=NewObject<UInputMappingContext>(this,TEXT("MouseInputMappingContextMap"));
    			
    			//创建1个触碰的UInputAction
    			
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
    			
    			//添加触碰的Pressed、Moved、Released的委托绑定
    			EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Started,this,&UTouchBaseComponent::Touch_Released_Internal_Event);
    			EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Triggered,this,&UTouchBaseComponent::Touch_Moved_Internal_Event);
    			EnhancedInputComponent->BindAction(Tem_IA_Float2D_Touch,ETriggerEvent::Completed,this,&UTouchBaseComponent::Touch_Released_Internal_Event);
    			
    			//添加到上下文中
    			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    			{
    				EnhancedInputLocalPlayerSubsystem->AddMappingContext(Touch_Input_Mapping_Context,InputPriority);
    			}
    		}
    	}
}

void UTouchBaseComponent::UnRegisterInputMapping()
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



void UTouchBaseComponent::Touch_Pressed_Internal_Event(const FInputActionInstance& Value)
{
}

void UTouchBaseComponent::Touch_Moved_Internal_Event(const FInputActionInstance& Value)
{
}

void UTouchBaseComponent::Touch_Released_Internal_Event(const FInputActionInstance& Value)
{
}


