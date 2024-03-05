// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InputBaseComponent.h"
#include "KeyboardInputComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKeyboardInputDelegate,float,Value);

UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
class COMMONGAMEPLAYTOOLPLUGIN_API UKeyboardInputComponent : public UInputBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UKeyboardInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/* 注册触碰输入映射 */
	virtual void RegisterInputMapping() override;
    
	/* 取消注册触碰输入映射 */
	virtual void UnRegisterInputMapping() override;
	
	/* 用来存放键盘输入相关的UInputMappingContext */
	UPROPERTY(BlueprintReadWrite,Category="UMouseInputComponent|Input")
	UInputMappingContext* KeyboardInputMappingContextMap=nullptr;

	/* 用来存放键盘输入相关的UInputAction的TMap */
	UPROPERTY(BlueprintReadWrite,Category="UMouseInputComponent|Input")
	TMap<FName,UInputAction*> KeyboardInputActionMap;

	/* 键盘A输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_A_Input;

	/* 键盘B输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_B_Input;

	/* 键盘C输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_C_Input;

	/* 键盘D输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_D_Input;

	/* 键盘E输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_E_Input;

	/* 键盘F输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UKeyboardInputComponent|Keyboard")
	FKeyboardInputDelegate Keyboard_F_Input;
	
	void Keyboard_A_Input_Internal(const FInputActionInstance& Value);
	void Keyboard_B_Input_Internal(const FInputActionInstance& Value);
	void Keyboard_C_Input_Internal(const FInputActionInstance& Value);
	void Keyboard_D_Input_Internal(const FInputActionInstance& Value);
	void Keyboard_E_Input_Internal(const FInputActionInstance& Value);
	void Keyboard_F_Input_Internal(const FInputActionInstance& Value);
	//....

	
};
