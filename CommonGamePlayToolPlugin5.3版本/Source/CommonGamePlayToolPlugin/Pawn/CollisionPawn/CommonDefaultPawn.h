// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CustomBasePawn.h"
#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CommonDefaultPawn.generated.h"

UENUM(BlueprintType)
enum class ECollisionShapeComponentType :uint8
{
	SPhere,//球形
	Capsule,//胶囊体
	Box		//盒形
};


/* 用于定义自定义的默认Pawn */
UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonDefaultPawn : public ACustomBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommonDefaultPawn(const FObjectInitializer& ObjectInitializer);


	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/* 用于定义要创建的胶囊体形状 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="ACommonCustomDefaultPawn")
	TEnumAsByte<ECollisionShapeComponentType>CollisionShapeComponentType=ECollisionShapeComponentType::SPhere;
	
	/* 用于存放UPawnMovementComponent组件,需要一个碰撞组件作为根组件 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="ACommonCustomDefaultPawn")
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	/* 用于存放碰撞组件 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="ACommonCustomDefaultPawn")
	TObjectPtr<UShapeComponent> CollisionComponent;

	/* 弹簧臂组件,使视角过渡更加平滑 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="ACommonCustomDefaultPawn")
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	/* 弹簧臂组件,使视角过渡更加平滑 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="ACommonCustomDefaultPawn")
	TObjectPtr<UCameraComponent>CameraComponent;
	
	/* 注册输入映射 */
	virtual void RegisterInputMappding_Implementation() override;

#pragma region 视野速度
public:	
	//转动视野速度,X用于用于水平方向,Y用于垂直方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ACommonDefaultPawn|Speed")
	FVector2D ViewTurnSpeed=FVector2D(45.0f,45.0f);
	
	//移动速度,X用于用于水平方向,Y用于垂直方向
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ACommonDefaultPawn|Speed")
	FVector2D PawnMoveSpeed=FVector2D(1.0f,1.0f);
#pragma endregion

#pragma region 碰撞体高度属性
	/* 碰撞体高度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ACommonDefaultPawn|Collision",meta=(EditCondition="CollisionShapeComponentType==ECollisionShapeComponentType::SPhere",EditConditionHides))
	float SphereRadius=35.f;

	/* 碰撞体高度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ACommonDefaultPawn|Collision",meta=(EditCondition="CollisionShapeComponentType==ECollisionShapeComponentType::Capsule",EditConditionHides))
	float CapsuleHeight=75.0f;

	/* 碰撞体高度 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ACommonDefaultPawn|Collision",meta=(EditCondition="CollisionShapeComponentType==ECollisionShapeComponentType::Capsule",EditConditionHides))
	float CapsuleRadius=30.0f;
	
#pragma endregion

#pragma region 视野属性
#pragma endregion
	
	
#pragma region 开关属性
public:
	//用于控制是否可以向空中移动
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="ACommonDefaultPawn|Speed")
	bool bCanMoveInSky=true;
	
#pragma endregion
	
	//接收输入用来旋转和移动
	virtual void ReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value) override;
	virtual void ReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value) override;
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonDefaultPawn")
	void ReceiveFloatInputUpEvent(const float& Value);

	
#pragma region 视角相关参数设置
public:
	UFUNCTION(BlueprintCallable,Category="ACommonDefaultPawn|Camera")
	void SetCameraFOV(const float CameraFOV);
#pragma endregion

	
};
