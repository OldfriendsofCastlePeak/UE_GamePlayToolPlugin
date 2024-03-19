// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "../GameMode/CommonBaseGameModeBase.h"
#include "CommonBaseManager.generated.h"

/*
* 该Actor用于作为Manager的基础类型
 */
UCLASS(Blueprintable,BlueprintType,Abstract)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBaseManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACommonBaseManager()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
		PrimaryActorTick.bCanEverTick = false;
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		if (UGameplayStatics::GetGameMode(this)->IsA(ACommonBaseGameModeBase::StaticClass()))
		{
			//先绑定Manger注册委托
			Cast<ACommonBaseGameModeBase>(UGameplayStatics::GetGameMode(this))->On_All_Manager_Registered.AddDynamic(this,&ACommonBaseManager::On_All_Manager_Register_Complete_Event);
		}
		
		Super::BeginPlay();

		//默认在BeginPlay中初始化
		Initialize_Common_Manager_Event();
	}

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override
	{
		Super::Tick(DeltaTime);
	}
	
	/* 该方法用于初始化Manager */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBaseManager")
	void Initialize_Common_Manager_Event();
	virtual void Initialize_Common_Manager_Event_Implementation(){}

	/* 该方法用于收到所有Manager注册完之后委托调用 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBaseManager")
	void On_All_Manager_Register_Complete_Event();
	virtual void On_All_Manager_Register_Complete_Event_Implementation(){}

	/* 开始的所有关卡加载完毕 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBaseManager")
	void StartLevelLoadEnd();
	virtual void StartLevelLoadEnd_Implementation(){}
};