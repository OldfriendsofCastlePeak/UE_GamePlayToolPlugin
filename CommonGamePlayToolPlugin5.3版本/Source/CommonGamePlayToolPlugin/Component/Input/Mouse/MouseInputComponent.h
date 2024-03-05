// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InputBaseComponent.h"
#include "MouseInputComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseInput_FVector2D,FVector2D,Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseInput_Float1D,float,Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseInput_Bool,bool,Value);

UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
class COMMONGAMEPLAYTOOLPLUGIN_API UMouseInputComponent : public UInputBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UMouseInputComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/* 用来存放鼠标输入相关的UInputMappingContext */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	UInputMappingContext* MouseInputMappingContextMap;

	/* 用来存放鼠标输入相关的UInputAction的TMap */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	TMap<FName,UInputAction*> MouseInputActionMap;

	/* 注册鼠标输入相关映射 */
	virtual void RegisterInputMapping() override;

	/* 移除鼠标输入相关映射 */
	virtual void UnRegisterInputMapping() override;
	
	/* 鼠标XY轴输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UMouseInputComponent|MouseInput")
	FMouseInput_FVector2D MouseXYInput;

	/* 鼠标滚轮输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UMouseInputComponent|MouseInput")
	FMouseInput_Float1D MouseWheelInput;

	/* 鼠标左键输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UMouseInputComponent|MouseInput")
	FMouseInput_Bool MouseLeftButtonInput;

	/* 鼠标右键输入委托 */
	UPROPERTY(BlueprintAssignable,Category="UMouseInputComponent|MouseInput")
	FMouseInput_Bool MouseRightButtonInput;
	
	void MouseXYMove_Internal(const FInputActionInstance& Value);
	void MouseWheel_Internal(const FInputActionInstance& Value);
	void MouseLeft_Internal(const FInputActionInstance& Value);
	void MouseRight_Internal(const FInputActionInstance& Value);
};
