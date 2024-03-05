// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../InputBaseComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "TouchBaseComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchActionOne_Param,FVector,Location);

UCLASS(Blueprintable,BlueprintType,ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
class COMMONGAMEPLAYTOOLPLUGIN_API UTouchBaseComponent : public UInputBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTouchBaseComponent();

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

	/* 用于存储触碰状态的数组 */
	UPROPERTY(BlueprintReadWrite, Category = "Tocuh|Input")
	TArray<uint8> Touch_State_Index;
	
	/* 用于存储UInputAction行为的UInputMappingContext */
	UPROPERTY(BlueprintReadWrite,Category="Tocuh|Input")
	UInputMappingContext* Touch_Input_Mapping_Context=nullptr;

	/* 用于存储UInputAction的数组,存储0~9的触碰 */
	UPROPERTY(BlueprintReadWrite,Category="Tocuh|Input")
	TMap<FName,UInputAction*>Touch_Input_Action_Map;

	/* 用于通知触碰开始的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionOne_Param On_Touch_Pressed;

	/* 用于通知触碰持续移动的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionOne_Param On_Touch_Moved;

	/* 用于通知触碰结束的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionOne_Param On_Touch_Released;
	
	//用于内部绑定的触碰按下方法
	virtual void Touch_Pressed_Internal_Event(const FInputActionInstance& Value);
	//用于内部绑定使用的触碰移动方法
	virtual void Touch_Moved_Internal_Event(const FInputActionInstance& Value);
	//用于内部绑定使用的触碰松开方法
	virtual void Touch_Released_Internal_Event(const FInputActionInstance& Value);
	
};
