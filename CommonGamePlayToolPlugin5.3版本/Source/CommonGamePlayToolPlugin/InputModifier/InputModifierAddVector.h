// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputModifierAddVector.generated.h"

/**
 * 该类用于修改操作,在输入返回值上相加一个FVector
 */
UCLASS(NotBlueprintable,meta = (DisplayName = "AddVector"))
class COMMONGAMEPLAYTOOLPLUGIN_API UInputModifierAddVector : public UInputModifier
{
	GENERATED_BODY()
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	FVector Vector = FVector::ZeroVector;
	
	/* 重写UInputModifier的ModifyRaw方法来实现修改 */
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput* PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override
	{
		//确保值的类型不为Boolean
		if (ensureMsgf(CurrentValue.GetValueType() != EInputActionValueType::Boolean, TEXT("Scale modifier doesn't support boolean values.")))
		{
			return CurrentValue.Get<FVector>() + Vector;
		}
		return CurrentValue;
	}
};
