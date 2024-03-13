// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CommonBaseGameModeBase.generated.h"

class ACommonBaseManager;

UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBaseGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACommonBaseGameModeBase()
	{
		// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
	
	//声明不带参数的委托类型用于通知
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGameMode_Delegate);
	
	/* 该委托用于通知所有Manager注册完毕 */
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite,Category="ACommonBaseGameModeBase")
	FGameMode_Delegate On_All_Manager_Registered;
	
	/* 用于储存多个Manager */
	UPROPERTY(BlueprintReadWrite,Category="ACommonBaseGameModeBase")
	TMap<TSubclassOf<ACommonBaseManager>,ACommonBaseManager*>ManagerMap;
	
	/* 用于记录需要注册的Manager类型数量 */
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="ACommonBaseGameModeBase")
	uint8 ManagerTypeNum=10;

	/* 该方法用于注册Manager的具体实现 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="ACommonBaseGameModeBase")
	void Register_Manager_Event(TSubclassOf<ACommonBaseManager> ManagerClass,ACommonBaseManager* ManagerObject);
	virtual void Register_Manager_Event_Implementation(TSubclassOf<ACommonBaseManager> ManagerClass,ACommonBaseManager* ManagerObject)
	{
		//向Map中添加ACommonBaseManager对象
		ManagerMap.Add(ManagerClass,ManagerObject);

		//满足条件时,通知注册完毕
		if (ManagerMap.Num()>=ManagerTypeNum) On_All_Manager_Registered.Broadcast();
	}

	/* 获取对应的Manager对象 */
	UFUNCTION(BlueprintPure,Category="ACommonBaseGameModeBase")
	ACommonBaseManager* Get_Specify_Manager(TSubclassOf<ACommonBaseManager> ManagerClass)
	{
		if (!(ManagerClass->IsValidLowLevel())) return nullptr;
		return *ManagerMap.Find(ManagerClass);
	}
	
};
