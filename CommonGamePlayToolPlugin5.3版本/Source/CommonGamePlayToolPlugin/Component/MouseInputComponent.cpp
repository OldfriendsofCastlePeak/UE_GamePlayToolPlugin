﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "MouseInputComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// Sets default values for this component's properties
UMouseInputComponent::UMouseInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMouseInputComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	//注册鼠标输入映射
	RegisterMouseInputMapping();
}


// Called every frame
void UMouseInputComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMouseInputComponent::RegisterMouseInputMapping()
{
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(this->GetOwner()->InputComponent))
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{

			//IA_Float2D_MouseXY用来处理鼠标在XY轴上的输入
			UInputAction* IA_Float2D_MouseXY=NewObject<UInputAction>(this,TEXT("IA_Float2D_MouseXY"));
			IA_Float2D_MouseXY->bConsumeInput=true;
			IA_Float2D_MouseXY->bTriggerWhenPaused=false;
			IA_Float2D_MouseXY->bReserveAllMappings=false;
			IA_Float2D_MouseXY->ValueType=EInputActionValueType::Axis2D;
			MouseInputActionMap.Add(TEXT("IA_Float2D_MouseXY"),IA_Float2D_MouseXY);
	
			//IA_Bool_MouseLeftButton用来处理鼠标左键的输入
			UInputAction* IA_Bool_MouseLeftButton=NewObject<UInputAction>(this,TEXT("IA_Bool_MouseLeftButton"));
			IA_Bool_MouseLeftButton->bConsumeInput=true;
			IA_Bool_MouseLeftButton->bTriggerWhenPaused=false;
			IA_Bool_MouseLeftButton->bReserveAllMappings=false;
			IA_Bool_MouseLeftButton->ValueType=EInputActionValueType::Boolean;
			MouseInputActionMap.Add(TEXT("IA_Bool_MouseLeftButton"),IA_Bool_MouseLeftButton);
        	    
			//IA_Bool_MouseRightButton用来处理鼠标右键的输入
			UInputAction* IA_Bool_MouseRightButton=NewObject<UInputAction>(this,TEXT("IA_Bool_MouseRightButton"));
			IA_Bool_MouseRightButton->bConsumeInput=true;
			IA_Bool_MouseRightButton->bTriggerWhenPaused=false;
			IA_Bool_MouseRightButton->bReserveAllMappings=false;
			IA_Bool_MouseRightButton->ValueType=EInputActionValueType::Boolean;
			MouseInputActionMap.Add(TEXT("IA_Bool_MouseRightButton"),IA_Bool_MouseRightButton);
	
			//IA_Float1D_MouseWheel用来处理滚动鼠标中间的输入
			UInputAction* IA_Float1D_MouseWheel=NewObject<UInputAction>(this,TEXT("IA_Float1D_MouseWheel"));
			IA_Float1D_MouseWheel->bConsumeInput=true;
			IA_Float1D_MouseWheel->bTriggerWhenPaused=false;
			IA_Float1D_MouseWheel->bReserveAllMappings=false;
			IA_Float1D_MouseWheel->ValueType=EInputActionValueType::Axis1D;
			MouseInputActionMap.Add(TEXT("IA_Float1D_MouseWheel"),IA_Float1D_MouseWheel);
        	    
			//MouseInputMappingContextMap 用来映射按键输入
			MouseInputMappingContextMap=NewObject<UInputMappingContext>(this,TEXT("MouseInputMappingContextMap"));
		



		
			//将IA_Float2D_MouseXY与鼠标在XY轴上的输入绑定,Y轴需要其值作为FVector2D的Y,因为值不能都存在FVector2D的X.
			MouseInputMappingContextMap->MapKey(IA_Float2D_MouseXY,EKeys::MouseX);
			FEnhancedActionKeyMapping& FEnhancedActionKeyMappingMouseY= MouseInputMappingContextMap->MapKey(IA_Float2D_MouseXY,EKeys::MouseY);
			FEnhancedActionKeyMappingMouseY.Modifiers.Add(NewObject<UInputModifierSwizzleAxis>());
			EnhancedInputComponent->BindAction(IA_Float2D_MouseXY,ETriggerEvent::Triggered,this,&UMouseInputComponent::MouseXYMove_Internal);
	
			//将IA_Bool_MouseLeft与鼠标左键的输入绑定
			MouseInputMappingContextMap->MapKey(IA_Bool_MouseLeftButton,EKeys::LeftMouseButton);
			EnhancedInputComponent->BindAction(IA_Bool_MouseLeftButton,ETriggerEvent::Started,this,&UMouseInputComponent::MouseLeft_Internal);
			EnhancedInputComponent->BindAction(IA_Bool_MouseLeftButton,ETriggerEvent::Completed,this,&UMouseInputComponent::MouseLeft_Internal);
			
			//将IA_Bool_MouseRight与鼠标右键的输入绑定
			MouseInputMappingContextMap->MapKey(IA_Bool_MouseRightButton,EKeys::RightMouseButton);
			EnhancedInputComponent->BindAction(IA_Bool_MouseRightButton,ETriggerEvent::Started,this,&UMouseInputComponent::MouseRight_Internal);
			EnhancedInputComponent->BindAction(IA_Bool_MouseRightButton,ETriggerEvent::Completed,this,&UMouseInputComponent::MouseRight_Internal);
			
			//将IA_Float1D_MouseWheel与鼠标滚动的输入绑定
			MouseInputMappingContextMap->MapKey(IA_Float1D_MouseWheel,EKeys::MouseWheelAxis);
			EnhancedInputComponent->BindAction(IA_Float1D_MouseWheel,ETriggerEvent::Triggered,this,&UMouseInputComponent::MouseWheel_Internal);


			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(MouseInputMappingContextMap,0);
			}
		}
	}
}

void UMouseInputComponent::UnRegisterMouseInputMapping()
{
	if (MouseInputMappingContextMap)
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigatorController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(MouseInputMappingContextMap);
			}
		}
	}
}

void UMouseInputComponent::MouseXYMove_Internal(const FInputActionInstance& Value)
{
	MouseXYInput.Broadcast(Value.GetValue().Get<FVector2D>());
}

void UMouseInputComponent::MouseWheel_Internal(const FInputActionInstance& Value)
{
	MouseWheelInput.Broadcast(Value.GetValue().Get<float>());
}

void UMouseInputComponent::MouseLeft_Internal(const FInputActionInstance& Value)
{
	MouseLeftButtonInput.Broadcast(Value.GetValue().Get<bool>());
}

void UMouseInputComponent::MouseRight_Internal(const FInputActionInstance& Value)
{
	MouseRightButtonInput.Broadcast(Value.GetValue().Get<bool>());
}

