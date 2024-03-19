// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "InputBaseComponent.generated.h"




/* 该类用于输入组件的基类类型 */
UCLASS(Abstract,Blueprintable,BlueprintType,ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
class COMMONGAMEPLAYTOOLPLUGIN_API UInputBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInputBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UInputBaseComponent|Register")
	bool bAuto_Register_Input_Mapping=true;

	//输入优先级
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UInputBaseComponent|Register")
	int32 InputPriority=0;

	/* 消耗输入 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UInputBaseComponent|Register")
	bool bConsumeInput=true;
	
	/* 注册触碰输入映射 */
	UFUNCTION(BlueprintCallable,Category="UInputBaseComponent|Register")
	virtual void RegisterInputMapping(){};
    
	/* 取消注册触碰输入映射 */
	UFUNCTION(BlueprintCallable,Category="UInputBaseComponent|Register")
	virtual void UnRegisterInputMapping(){};

	/* 检测APlayerController上是否有UInputBaseComponent组件 */
	UFUNCTION(BlueprintPure,Category="UInputBaseComponent")
	virtual bool PlayerController_Contain_InputBaseComponent()
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(this->GetOwner()->GetInstigator()))
		{
			if (UInputBaseComponent* InputBaseComponent= PlayerController->GetComponentByClass<UInputBaseComponent>())
			{
				return true;
			};
		}
		return false;
	}

	/* 检测组件的Owner上是否有UInputBaseComponent组件 */
	UFUNCTION(BlueprintPure,Category="UInputBaseComponent")
	virtual bool Actor_Contain_InputBaseComponent()
	{
		if (AActor* Actor=this->GetOwner())
		{
			if (UInputBaseComponent* InputBaseComponent= Actor->GetComponentByClass<UInputBaseComponent>())
			{
				return true;
			}
		}
		return false;
	}
};
