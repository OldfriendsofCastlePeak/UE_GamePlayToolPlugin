// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyboardInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"


// Sets default values for this component's properties
UKeyboardInputComponent::UKeyboardInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UKeyboardInputComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UKeyboardInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKeyboardInputComponent::RegisterInputMapping()
{
	//检测KeyboardInputMappingContextMap是否有效,防止多次创建
	if(!KeyboardInputMappingContextMap) KeyboardInputMappingContextMap=NewObject<UInputMappingContext>(this,TEXT("KeyboardInputMappingContextMap"));

	//检测数组是否有UInputAction,防止多次创建
	if (KeyboardInputActionMap.Num()==0)
	{
		//添加键盘输入W
		UInputAction* IA_Float2D_W=NewObject<UInputAction>(this,TEXT("IA_Float2D_W"));
		IA_Float2D_W->bConsumeInput=bConsumeInput;
		IA_Float2D_W->bTriggerWhenPaused=false;
		IA_Float2D_W->bReserveAllMappings=false;
		IA_Float2D_W->ValueType=EInputActionValueType::Axis2D;
		KeyboardInputActionMap.Add(TEXT("IA_Float2D_W"),IA_Float2D_W);
		KeyboardInputMappingContextMap->MapKey(IA_Float2D_W,EKeys::W);
		
		
		//添加键盘输入A
		UInputAction* IA_Float2D_A=NewObject<UInputAction>(this,TEXT("IA_Float2D_A"));
		IA_Float2D_A->bConsumeInput=bConsumeInput;
		IA_Float2D_A->bTriggerWhenPaused=false;
		IA_Float2D_A->bReserveAllMappings=false;
		IA_Float2D_A->ValueType=EInputActionValueType::Axis2D;
		KeyboardInputActionMap.Add(TEXT("IA_Float2D_A"),IA_Float2D_A);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingA=KeyboardInputMappingContextMap->MapKey(IA_Float2D_A,EKeys::A);
		FEnhancedActionKeyMappingA.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		FEnhancedActionKeyMappingA.Modifiers.Add(NewObject<UInputModifierNegate>());
		
		
		//添加键盘输入S
		UInputAction* IA_Float2D_S=NewObject<UInputAction>(this,TEXT("IA_Float2D_S"));
		IA_Float2D_S->bConsumeInput=bConsumeInput;
		IA_Float2D_S->bTriggerWhenPaused=false;
		IA_Float2D_S->bReserveAllMappings=false;
		IA_Float2D_S->ValueType=EInputActionValueType::Axis2D;
		KeyboardInputActionMap.Add(TEXT("IA_Float2D_S"),IA_Float2D_S);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingS= KeyboardInputMappingContextMap->MapKey(IA_Float2D_S,EKeys::S);
		FEnhancedActionKeyMappingS.Modifiers.Add(NewObject<UInputModifierNegate>());
		
		//添加键盘输入D
		UInputAction* IA_Float2D_D=NewObject<UInputAction>(this,TEXT("IA_Float2D_D"));
		IA_Float2D_D->bConsumeInput=bConsumeInput;
		IA_Float2D_D->bTriggerWhenPaused=false;
		IA_Float2D_D->bReserveAllMappings=false;
		IA_Float2D_D->ValueType=EInputActionValueType::Axis2D;
		KeyboardInputActionMap.Add(TEXT("IA_Float2D_D"),IA_Float2D_D);
		FEnhancedActionKeyMapping& FEnhancedActionKeyMappingD=KeyboardInputMappingContextMap->MapKey(IA_Float2D_D,EKeys::D);
		FEnhancedActionKeyMappingD.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
		
		//添加键盘输入E
		UInputAction* IA_Float1D_E=NewObject<UInputAction>(this,TEXT("IA_Float1D_E"));
		IA_Float1D_E->bConsumeInput=bConsumeInput;
		IA_Float1D_E->bTriggerWhenPaused=false;
		IA_Float1D_E->bReserveAllMappings=false;
		IA_Float1D_E->ValueType=EInputActionValueType::Axis1D;
		KeyboardInputActionMap.Add(TEXT("IA_Float1D_E"),IA_Float1D_E);
		KeyboardInputMappingContextMap->MapKey(IA_Float1D_E,EKeys::E);
		
		//添加键盘输入Q
		UInputAction* IA_Float1D_Q=NewObject<UInputAction>(this,TEXT("IA_Float1D_Q"));
		IA_Float1D_Q->bConsumeInput=bConsumeInput;
		IA_Float1D_Q->bTriggerWhenPaused=false;
		IA_Float1D_Q->bReserveAllMappings=false;
		IA_Float1D_Q->ValueType=EInputActionValueType::Axis1D;
		KeyboardInputActionMap.Add(TEXT("IA_Float1D_Q"),IA_Float1D_Q);
		KeyboardInputMappingContextMap->MapKey(IA_Float1D_Q,EKeys::Q).Modifiers.Add(NewObject<UInputModifierNegate>());
		
	}
	
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				//先检查一下,防止重复注册
				const TArray<TUniquePtr<FEnhancedInputActionEventBinding>>* ActionEventBindings= &EnhancedInputComponent->GetActionEventBindings();
				bool bHasRegister=false;
				for (int i=0;i<ActionEventBindings->Num();i++)
				{
					if (EnhancedInputComponent->GetActionEventBindings()[i].Get()->GetAction()==KeyboardInputActionMap.FindChecked(TEXT("IA_Float2D_W")))
					{
						bHasRegister=true;
						return;
					}
				}

				if (bHasRegister) return;
				
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float2D_W")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_Move_Input_Internal);
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float2D_A")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_Move_Input_Internal);
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float2D_S")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_Move_Input_Internal);
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float2D_D")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_Move_Input_Internal);
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float1D_E")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_UpDown_Input_Internal);
				EnhancedInputComponent->BindAction(KeyboardInputActionMap.FindChecked(TEXT("IA_Float1D_Q")),ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_UpDown_Input_Internal);

				//注册
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(KeyboardInputMappingContextMap,InputPriority);
				
			}
		}
	}	

}

void UKeyboardInputComponent::UnRegisterInputMapping()
{
	if (KeyboardInputMappingContextMap)
    	{
    		if (const APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
    		{
    			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
    			{
    				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(KeyboardInputMappingContextMap);
    				bAuto_Register_Input_Mapping=false;
    			}
    		}
    	}
}



void UKeyboardInputComponent::Keyboard_Move_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_WASD_Input.Broadcast(Value.GetValue().Get<FVector2D>());
}

void UKeyboardInputComponent::Keyboard_UpDown_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_EQ_Input.Broadcast(Value.GetValue().Get<float>());
}


