// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TouchBaseComponent.h"
#include "SingleTouchComponent.generated.h"

/*  该组件用于处理单个触碰的输入 */
UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
class COMMONGAMEPLAYTOOLPLUGIN_API USingleTouchComponent : public UTouchBaseComponent
{
	GENERATED_BODY()
public:
	// Sets default values for this component's properties
	USingleTouchComponent();

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



	
	/* 用于记录最后的触碰位置 */
	UPROPERTY(BlueprintReadWrite,Category = "USingleTouchComponent")
	FVector Last_Touch_Position=FVector(-1,-1,-1);
	
	//用于内部绑定的触碰按下方法
	virtual void Touch_Pressed_Internal_Event(const FInputActionInstance& Value) override;
	//用于内部绑定使用的触碰移动方法
	virtual void Touch_Moved_Internal_Event(const FInputActionInstance& Value) override;
	//用于内部绑定使用的触碰松开方法
	virtual void Touch_Released_Internal_Event(const FInputActionInstance& Value) override;
	
};
