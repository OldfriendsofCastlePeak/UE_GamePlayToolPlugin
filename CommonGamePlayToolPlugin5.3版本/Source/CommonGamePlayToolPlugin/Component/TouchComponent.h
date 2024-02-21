// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TouchComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchOneFingerActionDelegate,FVector2D,InMoveDirectionValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FTouchTwoFingerActionDelegate,float,InMoveDirectionValue);

/*
 * 该类主要用于触碰功能
 */
UCLASS(ClassGroup=(CommonGamePlay), meta=(BlueprintSpawnableComponent),DisplayName="CommonGamePlayTouchComponent")
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

	//单指移动
	UPROPERTY(BlueprintAssignable,Category="Tocuh")
	FTouchOneFingerActionDelegate OneFingerTouchMove;

	//双指缩放
	UPROPERTY(BlueprintAssignable,Category="Tocuh")
	FTouchTwoFingerActionDelegate TwoFingerTouchScale;

	//增强输入的优先级
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="EnhanceInput")
	int32 InputPriority=0;

	//触碰1的开始事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch1StartedEvent(const FInputActionInstance& InputValue);

	//触碰1的持续触发事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch1TriggeredEvent(const FInputActionInstance& InputValue);
	
	//触碰1的结束事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch1CompletedEvent(const FInputActionInstance& InputValue);
	
	//触碰2的开始事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch2StartedEvent(const FInputActionInstance& InputValue);
	
	//触碰2的持续触发事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch2TriggeredEvent(const FInputActionInstance& InputValue);
	
	//触碰2的结束事件
	UFUNCTION(BlueprintNativeEvent,Category="Touch1")
	void Touch2CompletedEvent(const FInputActionInstance& InputValue);
	

private:
	//触碰1
	UPROPERTY()
	UInputAction* IA_Float2D_Touch1;
	
	//触碰2
	UPROPERTY()
	UInputAction* IA_Float2D_Touch2;

	//用于存储UInputAction和按键映射的InputContext
	UPROPERTY()
	UInputMappingContext* Touch_Input_Mapping_Context;

	//将UInputAction和对应的Touch触碰行为映射,并绑定具体行为按键
	void AddInputActionToInputMappingContext();

	//将输入映射注册到增强输入子系统中
	void RegisterInputMappingContextToEnhanceInoput() const;

	bool bTouch1Press=false;
	bool bTouch2Press=false;

	FVector2D Touch1LastLocation;
	FVector2D Touch2LastLocation;
	
	FVector2D Touch1Direction;
	FVector2D Touch2Direction;

	
	
};
