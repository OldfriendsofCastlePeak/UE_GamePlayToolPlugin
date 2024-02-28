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

	// ...
	//注册按键输入映射
	RegisterMouseInputMapping();
}


// Called every frame
void UKeyboardInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UKeyboardInputComponent::RegisterMouseInputMapping()
{
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			//添加键盘输入A
			UInputAction* IA_Float1D_A=NewObject<UInputAction>(this,TEXT("IA_Float1D_A"));
			IA_Float1D_A->bConsumeInput=true;
			IA_Float1D_A->bTriggerWhenPaused=false;
			IA_Float1D_A->bReserveAllMappings=false;
			IA_Float1D_A->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_A"),IA_Float1D_A);

			//添加键盘输入B
			UInputAction* IA_Float1D_B=NewObject<UInputAction>(this,TEXT("IA_Float1D_B"));
			IA_Float1D_B->bConsumeInput=true;
			IA_Float1D_B->bTriggerWhenPaused=false;
			IA_Float1D_B->bReserveAllMappings=false;
			IA_Float1D_B->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_B"),IA_Float1D_B);

			//添加键盘输入C
			UInputAction* IA_Float1D_C=NewObject<UInputAction>(this,TEXT("IA_Float1D_C"));
			IA_Float1D_C->bConsumeInput=true;
			IA_Float1D_C->bTriggerWhenPaused=false;
			IA_Float1D_C->bReserveAllMappings=false;
			IA_Float1D_C->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_C"),IA_Float1D_C);

			//添加键盘输入D
			UInputAction* IA_Float1D_D=NewObject<UInputAction>(this,TEXT("IA_Float1D_D"));
			IA_Float1D_D->bConsumeInput=true;
			IA_Float1D_D->bTriggerWhenPaused=false;
			IA_Float1D_D->bReserveAllMappings=false;
			IA_Float1D_D->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_D"),IA_Float1D_D);

			//添加键盘输入E
			UInputAction* IA_Float1D_E=NewObject<UInputAction>(this,TEXT("IA_Float1D_E"));
			IA_Float1D_E->bConsumeInput=true;
			IA_Float1D_E->bTriggerWhenPaused=false;
			IA_Float1D_E->bReserveAllMappings=false;
			IA_Float1D_E->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_E"),IA_Float1D_E);
			
			//添加键盘输入F
			UInputAction* IA_Float1D_F=NewObject<UInputAction>(this,TEXT("IA_Float1D_F"));
			IA_Float1D_F->bConsumeInput=true;
			IA_Float1D_F->bTriggerWhenPaused=false;
			IA_Float1D_F->bReserveAllMappings=false;
			IA_Float1D_F->ValueType=EInputActionValueType::Axis1D;
			KeyboardInputActionMap.Add(TEXT("IA_Float1D_F"),IA_Float1D_F);

			//MouseInputMappingContextMap 用来映射按键输入
			KeyboardInputMappingContextMap=NewObject<UInputMappingContext>(this,TEXT("KeyboardInputMappingContextMap"));

			//A
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::A);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_A_Input_Internal);

			//B
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::B);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_B_Input_Internal);

			//C
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::C);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_C_Input_Internal);

			//D
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::D);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_D_Input_Internal);

			//E
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::E);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_E_Input_Internal);

			//F
			KeyboardInputMappingContextMap->MapKey(IA_Float1D_A,EKeys::F);
			EnhancedInputComponent->BindAction(IA_Float1D_A,ETriggerEvent::Triggered,this,&UKeyboardInputComponent::Keyboard_F_Input_Internal);


			
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(KeyboardInputMappingContextMap,0);
			}









			
			
		}
	}	
}

void UKeyboardInputComponent::UnRegisterMouseInputMapping()
{
	if (KeyboardInputMappingContextMap)
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(KeyboardInputMappingContextMap);
			}
		}
	}
}

void UKeyboardInputComponent::Keyboard_A_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_A_Input.Broadcast(Value.GetValue().Get<float>());
}

void UKeyboardInputComponent::Keyboard_B_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_B_Input.Broadcast(Value.GetValue().Get<float>());
}

void UKeyboardInputComponent::Keyboard_C_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_C_Input.Broadcast(Value.GetValue().Get<float>());
}

void UKeyboardInputComponent::Keyboard_D_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_D_Input.Broadcast(Value.GetValue().Get<float>());
}

void UKeyboardInputComponent::Keyboard_E_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_E_Input.Broadcast(Value.GetValue().Get<float>());
}

void UKeyboardInputComponent::Keyboard_F_Input_Internal(const FInputActionInstance& Value)
{
	Keyboard_F_Input.Broadcast(Value.GetValue().Get<float>());
}

