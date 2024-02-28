// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TouchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchInputDelegate,FVector,Value);


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
class COMMONGAMEPLAYTOOLPLUGIN_API UTouchComponent : public UActorComponent
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

	/* 用于存储UInputAction行为的UInputMappingContext */
	UPROPERTY(BlueprintReadWrite,Category="Tocuh|Input")
	class UInputMappingContext* Touch_Input_Mapping_Context=nullptr;

	/* 用于存储UInputAction的数组 */
	UPROPERTY(BlueprintReadWrite,Category="Tocuh|Input")
	TMap<FName,class UInputAction*>Touch_Input_Action_Array;
	
	/* 注册触碰输入映射 */
	void RegisterTouchInput();

	/* 取消注册触碰输入映射 */
	void UnRegisterTouchInput();
	
	/* 触碰手指1的开始委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_1_Start;

	/* 触碰手指1的移动委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_1_Moving;

	/* 触碰手指1的松开委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_1_End;

	/* 触碰手指2的开始委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_2_Start;

	/* 触碰手指2的移动委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_2_Moving;

	/* 触碰手指2的松开委托 */
	UPROPERTY(BlueprintAssignable,Category="UTouchComponent|Touch1")
	FTouchInputDelegate Touch_2_End;



	bool bTouch1Press=false;
	bool bTouch2Press=false;

	FVector2D Touch1LastLocation;
	FVector2D Touch2LastLocation;
	
	FVector2D Touch1Direction;
	FVector2D Touch2Direction;





	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTouchDynmic, FVector, Moved);



	/** * 多播指定接收到的调度器1 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch1;

	/** * 多播指定接收到的调度器2 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch2;

	/** * 多播指定接收到的调度器3 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch3;

	/** * 多播指定接收到的调度器4 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch4;

	/** * 多播指定接收到的调度器5 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch5;

	/** * 多播指定接收到的调度器6 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch6;

	/** * 多播指定接收到的调度器7 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch7;

	/** * 多播指定接收到的调度器8 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch8;

	/** * 多播指定接收到的调度器9 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch9;

	/** * 多播指定接收到的调度器10 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnTouchDynmic OnMovedTouch10;


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnIndexTouchDynmic,FVector, Moved, uint8, FingerIndex);

	/** * 多播收到触发的调度器 */
	UPROPERTY(BlueprintAssignable, Category = "Aimo|On")
	FOnIndexTouchDynmic OnTriggerTouch;







	
	/** * 绑定触控 */
	UFUNCTION(BlueprintCallable, Category = "Aimo|Function")
	virtual bool DelegateBind(uint8 FingerIndex, bool bDelegateBind, UObject* InFunctionObject, const FName& InFunctionName);


private:
	void Touch1Pressed_Internal(const FInputActionInstance& Value);
	void Touch1Moved_Internal(const FInputActionInstance& Value);
	void Touch1Released_Internal(const FInputActionInstance& Value);
	
	void Touch2Pressed_Internal(const FInputActionInstance& Value);
	void Touch2Moved_Internal(const FInputActionInstance& Value);
	void Touch2Released_Internal(const FInputActionInstance& Value);

	

	
};

