// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputAction.h"
#include "TouchBaseComponent.h"
#include "TouchComponent.generated.h"


//定义两个方法的触碰委托类型
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FTouchActionTwo_Param,FVector,Location,uint8,FingerIndex);


/* 该枚举类型用于不同的触碰操作 */
UENUM(BlueprintType)
enum ETouchActionMode:uint8
{
	TouchGame		UMETA(Displayname="射击类手游"),		//左手区域控制移动,右手区域控制旋转
	TouchOnePerson	UMETA(Displayname="第一人称观光模式"),	//单指控制方向,双指控制缩放
	//...后序在此扩展
	Unkown			UMETA(Displayname="未知")
};

/*
 * 该类主要用于触碰功能
 */
UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent),DisplayName="CommonGamePlayTouchComponent")
class COMMONGAMEPLAYTOOLPLUGIN_API UTouchComponent : public UTouchBaseComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTouchComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	/* 注册触碰输入映射 */
    void RegisterTouchInput();
    
    /* 取消注册触碰输入映射 */
    void UnRegisterTouchInput();
	

	
	
	/* 用于通知触碰开始的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionTwo_Param On_Touch_Pressed_TwoParam;

	/* 用于通知触碰持续移动的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionTwo_Param On_Touch_Moved_TwoParam;

	/* 用于通知触碰结束的委托 */
	UPROPERTY(BlueprintAssignable,Category="Tocuh|Input")
	FTouchActionTwo_Param On_Touch_Released_TwoParam;

	/* 获取需要释放的触控索引数组 */
	UFUNCTION(BlueprintPure, Category = "Tocuh|Input")
	virtual TArray<uint8> Get_Should_Released_Touch_Input_Array();
	
public:
	//用于内部绑定的触碰按下方法
	void Touch_Pressed_Internal_Event(const FInputActionInstance& Value);
	//用于内部绑定使用的触碰移动方法
	void Touch_Moved_Internal_Event(const FInputActionInstance& Value);
	//用于内部绑定使用的触碰松开方法
	void Touch_Released_Internal_Event(const FInputActionInstance& Value);
	
};

