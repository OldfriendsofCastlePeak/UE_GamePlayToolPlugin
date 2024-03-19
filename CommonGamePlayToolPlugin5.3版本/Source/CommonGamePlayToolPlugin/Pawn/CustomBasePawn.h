// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CustomBasePawn.generated.h"

//定义的一个自定义的基类类型,定义方向供其他子类具体实现
UCLASS(Abstract,BlueprintType,Blueprintable)
class COMMONGAMEPLAYTOOLPLUGIN_API ACustomBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomBasePawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
	{
		// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = true;
	}
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

	//用来注册输入映射
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="BasePawn")
	void RegisterInputMappding();
	virtual void RegisterInputMappding_Implementation(){}

	//接收用于移动的输入
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="BasePawn")
	void ReceiveFVector2DInputMoveEvent(const FVector2D& Value);
	virtual void ReceiveFVector2DInputMoveEvent_Implementation(const FVector2D& Value){};

	//接收用于旋转的输入
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="BasePawn")
	void ReceiveFVector2DInputRotateEvent(const FVector2D& Value);
	virtual void ReceiveFVector2DInputRotateEvent_Implementation(const FVector2D& Value){};
	
};
