// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIBaseCharacter.generated.h"

UCLASS(Abstract,BlueprintType,Blueprintable)
class COMMONGAMEPLAYTOOLPLUGIN_API AAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//声明用于通知的委托类型
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAINotifyDelegate,uint8,Value);

	/* AI移动结束 */
	UPROPERTY(BlueprintAssignable,BlueprintReadWrite,EditAnywhere)
	FAINotifyDelegate AIMoveEnd;

	UPROPERTY(BlueprintReadWrite,EditAnywhere)
	uint8 Value;
	
	UFUNCTION(BlueprintCallable)
	void CallAIMoveEndDelegate()
	{
		AIMoveEnd.Broadcast(Value);
	}
	
	/* AI从一个位置移动到另一个位置 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void AIMoveFromStartToEnd(const FVector StartLoction,const FVector EndLoction);
};
