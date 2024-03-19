// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ReceiveInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UReceiveInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class COMMONGAMEPLAYTOOLPLUGIN_API IReceiveInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/* 接收输入事件 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ReceiveInputInterface")
	void ReceiveInputEvent(const FVector2D& Value);
	virtual void ReceiveInputEvent_Implementation(const FVector2D& Value){}
	
	/* 接收输入进行旋转事件 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ReceiveInputInterface")
	void ReceiveInputRotateEvent(const FVector2D& Value);
	virtual void ReceiveInputRotateEvent_Implementation(const FVector2D& Value){}

	/* 接收输入进行移动事件 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ReceiveInputInterface")
	void ReceiveInputMoveEvent(const FVector2D& Value);
	virtual void ReceiveInputMoveEvent_Implementation(const FVector2D& Value){}

	/* 接收输入进行视角缩放事件 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ReceiveInputInterface")
	void ReceiveInputViewZoomEvent(const float Value);
	virtual void ReceiveInputViewZoomEvent_Implementation(const float Value){}
};
