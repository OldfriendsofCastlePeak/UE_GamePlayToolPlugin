﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Pawn.h"
#include "ViewBlendComponent.generated.h"



UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent),Blueprintable)
class COMMONGAMEPLAYTOOLPLUGIN_API UViewBlendComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UViewBlendComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	/* 声明一个委托类型 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FViewToolDelegate);
	
	/* 当开始调用"Set View Target With Blend"执行的委托 */
	UPROPERTY(BlueprintAssignable,Category="ViewToolComponent",meta = (DeprecatedProperty, DeprecationMessage = "委托待绑定,暂时不可使用"))
	FViewToolDelegate OnBeginViewBlend;

	/* 当结束调用"Set View Target With Blend"执行的委托 */
	UPROPERTY(BlueprintAssignable,Category="ViewToolComponent")
	FViewToolDelegate OnEndViewBlend;

	/* 更新 CurrentPlayerIndex 属性 */
	UFUNCTION(Blueprintable,Category="ViewToolComponent")
	void UpdateCurrentPlayerIndex();

	/* 获取 CurrentPlayerIndex 属性 */
	int GetCurrentPlayerIndex() const;
	
	/* 绑定委托 */
	UFUNCTION(Blueprintable,Category="ViewToolComponent")
	void BindDelegate();

	/* 解绑委托 */
	UFUNCTION(Blueprintable,Category="ViewToolComponent")
	void UnBindDelegate();
	
	
private:
	int CurrentPlayerIndex=0;
};


UCLASS()
class ABlendViewPawn : public APawn
{
	GENERATED_BODY()
public:
	//ACommonBasePawn(const FObjectInitializer& ObjectInitializer);
	// Sets default values for this pawn's properties
	ABlendViewPawn(const FObjectInitializer& ObjectInitializer)
	 :Super(ObjectInitializer)
	{
		Scene=CreateDefaultSubobject<USceneComponent>("Root");
		SetRootComponent(Scene);
		Camera=CreateDefaultSubobject<UCameraComponent>("ABlendViewPawnCamera");
		Camera->SetupAttachment(Scene);
	}
	UPROPERTY()
	USceneComponent* Scene=nullptr;
	
	UPROPERTY()
	UCameraComponent* Camera=nullptr;
	
};
















