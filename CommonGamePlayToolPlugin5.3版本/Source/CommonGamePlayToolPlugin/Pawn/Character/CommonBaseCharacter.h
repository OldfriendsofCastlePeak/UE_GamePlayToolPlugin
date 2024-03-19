// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Interface/ReceiveInputInterface.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CommonBaseCharacter.generated.h"

UCLASS()
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBaseCharacter : public ACharacter,public IReceiveInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACommonBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
	
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override
	{
		Super::SetupPlayerInputComponent(PlayerInputComponent);
		RegisterInputMappding();
	}

#pragma region Character的额外组件
public:
	/* 用于存放USpringArmComponent组件 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBaseCharacter")
	TObjectPtr<USpringArmComponent>SpringArmComponent;

	/* 用于存放UCameraComponent */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBaseCharacter")
	TObjectPtr<UCameraComponent>CameraComponent;
#pragma endregion
	
#pragma region Character的参数属性
public:
	/* 移动速度 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBaseCharacter")
	FVector2D MoveSpeed=FVector2D::One();

	/* 旋转速度 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBaseCharacter")
	FVector2D RotateSpeed=FVector2D::One();

	/* 旋转范围 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="ACommonBaseCharacter")
	FVector2D RotateRange=FVector2D(-89.0f,89.0f);
	
#pragma endregion
	//用来注册输入映射
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="BasePawn")
	void RegisterInputMappding();
	virtual void RegisterInputMappding_Implementation(){}

	/* 接收输入进行旋转事件 */
	virtual void ReceiveInputRotateEvent_Implementation(const FVector2D& Value)override;


	/* 接收输入进行旋转事件 */
    virtual void ReceiveInputMoveEvent_Implementation(const FVector2D& Value)override;

#pragma region 内部使用函数
private:
	FRotator inline GetCurrentCameraRotate() const;
	
#pragma endregion
	
	
};
