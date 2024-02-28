// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"


// Sets default values for this component's properties
UTouchComponent::UTouchComponent()
{
	//Tick没必要开启
	PrimaryComponentTick.bCanEverTick = false;
	
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
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
    	{
    		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
    		{
    			//IA_Float2D_Touch1用来处理Touch1的输入
    			UInputAction* IA_Float2D_Touch1=NewObject<UInputAction>(this,TEXT("IA_Float2D_Touch1"));
    			IA_Float2D_Touch1->bConsumeInput=true;
    			IA_Float2D_Touch1->bTriggerWhenPaused=false;
    			IA_Float2D_Touch1->bReserveAllMappings=false;
    			IA_Float2D_Touch1->ValueType=EInputActionValueType::Axis2D;
    			Touch_Input_Action_Array.Add(TEXT("IA_Float2D_Touch1"),IA_Float2D_Touch1);
    	
    			//IA_Float2D_Touch1用来处理Touch2的输入
    			UInputAction* IA_Float2D_Touch2=NewObject<UInputAction>(this,TEXT("IA_Float2D_Touch2"));
    			IA_Float2D_Touch2->bConsumeInput=true;
    			IA_Float2D_Touch2->bTriggerWhenPaused=false;
    			IA_Float2D_Touch2->bReserveAllMappings=false;
    			IA_Float2D_Touch2->ValueType=EInputActionValueType::Axis2D;
    			Touch_Input_Action_Array.Add(TEXT("IA_Float2D_Touch2"),IA_Float2D_Touch2);
            	    
    			//Touch_Input_Mapping_Context用来映射触碰输入
    			Touch_Input_Mapping_Context=NewObject<UInputMappingContext>(this,TEXT("MouseInputMappingContextMap"));
    		
    
    		
    			//将IA_Float2D_Touch1与Touch1的输入绑定
    			Touch_Input_Mapping_Context->MapKey(IA_Float2D_Touch1,EKeys::TouchKeys[0]);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch1,ETriggerEvent::Started,this,&UTouchComponent::Touch1Pressed_Internal);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch1,ETriggerEvent::Triggered,this,&UTouchComponent::Touch1Pressed_Internal);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch1,ETriggerEvent::Completed,this,&UTouchComponent::Touch1Released_Internal);
    	
    			//将IA_Float2D_Touch2与Touch2的输入绑定
    			Touch_Input_Mapping_Context->MapKey(IA_Float2D_Touch2,EKeys::TouchKeys[1]);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch2,ETriggerEvent::Started,this,&UTouchComponent::Touch2Pressed_Internal);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch2,ETriggerEvent::Triggered,this,&UTouchComponent::Touch2Pressed_Internal);
    			EnhancedInputComponent->BindAction(IA_Float2D_Touch2,ETriggerEvent::Completed,this,&UTouchComponent::Touch2Released_Internal);

    			
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

bool UTouchComponent::DelegateBind(uint8 FingerIndex, bool bDelegateBind, UObject* InFunctionObject,
	const FName& InFunctionName)
{
	if (InFunctionObject == nullptr)
	{
		return false;
	}
	FScriptDelegate ScriptDelegate; //建立对接变量
	ScriptDelegate.BindUFunction(InFunctionObject, InFunctionName); //对接变量绑定函数
	switch (FingerIndex)
	{
		case 0:
			if (bDelegateBind)
			{
				OnMovedTouch1.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch1.Remove(ScriptDelegate);
			}
			break;
		case 1:
			if (bDelegateBind)
			{
				OnMovedTouch2.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch2.Remove(ScriptDelegate);
			}
			break;
		case 2:
			if (bDelegateBind)
			{
				OnMovedTouch3.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch3.Remove(ScriptDelegate);
			}
			break;
		case 3:
			if (bDelegateBind)
			{
				OnMovedTouch4.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch4.Remove(ScriptDelegate);
			}
			break;
		case 4:
			if (bDelegateBind)
			{
				OnMovedTouch5.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch5.Remove(ScriptDelegate);
			}
			break;
		case 5:
			if (bDelegateBind)
			{
				OnMovedTouch6.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch6.Remove(ScriptDelegate);
			}
			break;
		case 6:
			if (bDelegateBind)
			{
				OnMovedTouch7.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch7.Remove(ScriptDelegate);
			}
			break;
		case 7:
			if (bDelegateBind)
			{
				OnMovedTouch8.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch8.Remove(ScriptDelegate);
			}
			break;
		case 8:
			if (bDelegateBind)
			{
				OnMovedTouch9.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch9.Remove(ScriptDelegate);
			}
			break;
		case 9:
			if (bDelegateBind)
			{
				OnMovedTouch10.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnMovedTouch10.Remove(ScriptDelegate);
			}
			break;
		case 10:
			if (bDelegateBind)
			{
				OnTriggerTouch.Add(ScriptDelegate); //绑定对接变量
			}
			else
			{
				OnTriggerTouch.Remove(ScriptDelegate);
			}
			break;
		default:
			FingerIndex = 255;
			break;
	}
	return FingerIndex != 255;
}

void UTouchComponent::Touch1Pressed_Internal(const FInputActionInstance& Value)
{
	Touch_1_Start.Broadcast(Value.GetValue().Get<FVector>());
}

void UTouchComponent::Touch1Moved_Internal(const FInputActionInstance& Value)
{
	Touch_1_Moving.Broadcast(Value.GetValue().Get<FVector>());
}

void UTouchComponent::Touch1Released_Internal(const FInputActionInstance& Value)
{
	Touch_1_End.Broadcast(Value.GetValue().Get<FVector>());
}

void UTouchComponent::Touch2Pressed_Internal(const FInputActionInstance& Value)
{
	Touch_2_Start.Broadcast(Value.GetValue().Get<FVector>());
}

void UTouchComponent::Touch2Moved_Internal(const FInputActionInstance& Value)
{
	Touch_2_Moving.Broadcast(Value.GetValue().Get<FVector>());
}

void UTouchComponent::Touch2Released_Internal(const FInputActionInstance& Value)
{
	Touch_2_End.Broadcast(Value.GetValue().Get<FVector>());
}




