// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	/* 父类UI */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		TObjectPtr<UWidget> ParentWidget;

	/* 本地位置,包括嵌套布局后的位置 */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		FVector2D LocalWidgetPosition;

	/* 自定义偏移位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		FVector2D CustomOffsetPosition;

	/* 触发偏移位置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		FVector2D TriggerOffsetPosition;

	/* 最后触发位置 */
	UPROPERTY(BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		FVector LastTriggerLocation;

	/** 禁用图片设置 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		FSlateBrush DisabledSlateBrush;

	/** * 自定义触发 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		bool bCustomTrigger;

	/** * 禁用 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UTouchBaseWidget|Variable")
		bool bDisabled = false;
	
	

	/* 所有接收到的调度器 */
	UPROPERTY(BlueprintAssignable, Category = "UTouchBaseWidget|On")
		FOnPressed OnTouchLocation;

protected:
	
	virtual void NativePreConstruct() override;

	// Called when the game starts
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized() override;


public:
#pragma region 获取UI大小信息

	/**
	 * 获取控件基本大小(设计中的大小).
	 * @return 返回控件基本大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	FVector2D GetWidgetAbsolutionSize();
	
	/**
	 * 获取控件基本大小(设计中的大小).
	 * @return 返回控件基本大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	FVector2D GetWidgetBaseSize();

	/**
	 * 获取控件受DPI影响的大小,例如:DPI设置的1080的DPI为1,则1440的DPI为1.333333,此时大小则为设计中的UI大小*1.333333
	 * @return 返回控件的显示大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	FVector2D GetWidgetDPISize();
	
	/**
	 * 获取控件的显示大小,计算公式为:UI设计大小*DPI*UI的Tansform的Scale属性
	 * @return 返回控件的显示大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	FVector2D GetWidgetShowSize();
	
#pragma endregion

#pragma region 获取UI位置信息
	/**/
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	FVector2D GetWidgetAbsolutePosition();
	
	/**
	 * 获取UI在本地空间中的位置,测试发现锚点在左上(0,0)时,返回的结果为UI的位置加其对应的宽高,
	 * 例如:位置为X:100,Y:200,宽为100,高为100,传入X:0.5,Y:0返回X:100+100*0.5,Y:200+100*0
	 * @param InVector 
	 * @return 返回UI在本地空间中的位置
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	FVector2D GetWidgetInLocalCoordinatesLocation(const FVector2D InVector);

	/**
	 * 获取本地位置,包括嵌套布局后的偏移
	 * @return 
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	virtual FVector2D GetLocalPosition();

	
#pragma endregion
	
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

	/** * 设置禁用  */
	UFUNCTION(BlueprintCallable, Category = "UTouchBaseWidget|Function")
		virtual void SetDisabled(bool bIsDisabled);

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
