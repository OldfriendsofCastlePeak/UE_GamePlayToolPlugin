// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Lib/TouchUILib.h"
#include "TouchBaseWidget.generated.h"

/* 多播指定接收到的调度器 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPressed, FVector, Location);

/**
 * 触碰UI的基本类型
 */
UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API UTouchBaseWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	/* 用于指示UI是否禁用触碰输入 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	bool bDisabled = false;
	
	/* UI的父级UI */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	TObjectPtr<UWidget> ParentWidget;

	/* UI的本地位置,包括嵌套布局后的位置 */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	FVector2D LocalWidgetPosition;

	/* 自定义偏移位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	FVector2D CustomOffsetPosition;

	/* 触发偏移位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	FVector2D TriggerOffsetPosition;

	/* 最后触发位置 */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	FVector LastTriggerLocation;

	/* UI图片设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	FSlateBrush DisabledSlateBrush;

	/* 自定义触发 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|TouchInput")
	bool bCustomTrigger;
	
	/* 所有接收到的调度器 */
	UPROPERTY(BlueprintAssignable, Category = "UTouchBaseWidget|On")
	FOnPressed OnTouchLocation;

protected:
	
	virtual void NativePreConstruct() override;

	// Called when the game starts
	virtual void NativeConstruct() override;

	//UI的初始化事件,顺序为NativeOnInitialized->NativePreConstruct->NativeConstruct
	virtual void NativeOnInitialized() override;

	
public:
#pragma region 获取UI位置信息
	/**
	* 获取本地位置,包括嵌套布局后的偏移
	* @return 
	*/
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	FVector2D GetLocalPosition();

	
#pragma endregion
	/* 用于绑定到UTouchComponent来接收触碰输入使用 */
	UFUNCTION()
	virtual void Touch_Pressed_Internal(const FVector& Location, uint8 FingerIndex);

	/* 用于绑定到UTouchComponent来接收触碰输入使用 */
	UFUNCTION()
	virtual void Touch_Moved_Internal(const FVector& Location, uint8 FingerIndex);

	/* 用于绑定到UTouchComponent来接收触碰输入使用 */
	UFUNCTION()
	virtual void Touch_Released_Internal(const FVector& Location, uint8 FingerIndex);
	
	/* 绑定触碰委托到UI */
	UFUNCTION(BlueprintCallable, Category = "Function")
		virtual void BindTouchDelegate();

	/* 接收触碰委托的位置和索引 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
	virtual void TouchIndexLocation(const FVector& Location, uint8 FingerIndex);

	/** * 设置触控是否绑定 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual void SetIndexTouchDelegate(bool bDelegateBind, uint8 FingerIndex);

	/** * 触发移动位置 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual void TouchMovedLocation(const FVector& Location);

	

	/* 判断是否进入触控区域 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual bool IsTouchLocation(const FVector& Location);



	/** * 播放动画 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual void TriggerInedxAnimation(int Index);

	/** * 蓝图播放动画 */
	UFUNCTION(BlueprintImplementableEvent, Category = "UTouchBaseWidget|Function")
		void BPTriggerInedxAnimation(int Index);

	/** * 组件销毁回调 */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual void ComponentDeactivated(UActorComponent* ActorComponent);
};
