// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "GameFramework/Pawn.h"
#include "CommonBasePawn.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputAction_FVector2D,FVector2D,Value);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputAction_Float,float,Value);
// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputAction_Bool,bool,Value);


UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommonBasePawn(const FObjectInitializer& ObjectInitializer);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region Pawn默认的相关组件
	/* Actor的根组件 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Input")
	USceneComponent* Scene;

	/* 弹簧臂组件,用于鸟瞰形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Input")
	class USpringArmComponent* SpringArm;

	/* 挂在弹簧臂上的相机,用于鸟瞰形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Input")
	class UCameraComponent* SpringArm_Camera;

	/* 用于第一人称世界形式的移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Input")
	UCameraComponent* Normal_Camera;
#pragma endregion
	
#pragma region 处理相关功能需要的属性
	/* bL_Mouse_Pressed用于条件判断,用于当鼠标左键按下时才执行的行为 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Mouse")
	bool bL_Mouse_Pressed=false;

	/* bR_Mouse_Pressed用于条件判断,用于当鼠标右键按下时才执行的行为 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn|Mouse")
	bool bR_Mouse_Pressed=false;

	/* bCan_Move用于条件判断,控制Pawn是否可以在接收输入之后进行移动 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBasePawn")
	bool bCan_Move=true;

	





#pragma endregion

#pragma region 输入事件输入相关
	/* 用来存放UInputMappingContext的TMap,使用一个UInputMappingContext即可,为了扩展考虑,这里使用TMap */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	TMap<FName,UInputMappingContext*> InputMappingContextMap;

	/* 用来存放UInputAction的TMap */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBasePawn|Input")
	TMap<FName,UInputAction*> InputActionMap;

	/* 鼠标XY轴移动事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseXYMoveEvent(const FVector2D Value);

	/* 键盘按键WASD移动事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void KeyboardWASDMoveEvent(const FVector2D Value);

	/* 键盘按键EQ委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void KeyboardEQUpEvent(const float Value);

	/* 鼠标古滚轮委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseWheelEvent(const float Value);

	/* 鼠标左键委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseLeftButtonEvent(const bool Value);

	/* 鼠标右键委托事件 */
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="ACommonBasePawn|Input")
	void MouseRightButtonEvent(const bool Value);

	/* 添加增强输入映射 */
	UFUNCTION(Blueprintable,Category="ACommonBasePawn|Input")
	void AddEnhancedContext();
	
private:	
	void MouseXYMove_Internal(const FInputActionInstance& Value);
	void KeyboardWASDMove_Internal(const FInputActionInstance& Value);
	void KeyboardEQUp_Internal(const FInputActionInstance& Value);
	void MouseWheel_Internal(const FInputActionInstance& Value);
	void MouseLeft_Internal(const FInputActionInstance& Value);
	void MouseRight_Internal(const FInputActionInstance& Value);
#pragma endregion

#pragma region 处理Pawn移动相关
	/* 当SpringArm_Camera相机激活,且Normal_Camera相机未激活时调用 */
	void OnOnlySpringArm_Camera_Active_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机激活,且SpringArm_Camera相机未激活时调用 */
	void OnOnlyNormal_Camera_Active_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机和SpringArm_Camera相机都未激活时调用 */
	void OnNo_Camera_Active_Internal(const FVector2D& Value);

	/* 当Normal_Camera相机和SpringArm_Camera相机都激活时调用 */
	void OnAll_Camera_Active_Internal(const FVector2D& Value);
	
	/* 仅当鼠标左键按下,且鼠标右键未按下时调用 */
	void OnOnlyMouseLeftButtonPressed_Internal(const FVector2D& Value);

	/* 仅当鼠标右键按下,且鼠标左键未按下时调用 */
	void OnOnlyMouseRightButtonPressed_Internal(const FVector2D& Value);

	/* 当鼠标左键与右键都按下时调用 */
	void OnMouseAllButtonPressed_Internal(const FVector2D& Value);

#pragma endregion

	
};
