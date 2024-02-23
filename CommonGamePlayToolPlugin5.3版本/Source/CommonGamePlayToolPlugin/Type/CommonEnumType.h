#pragma once
#include "CoreMinimal.h"
#include "CommonEnumType.generated.h"

/* 用于声明移动方式类型 */
UENUM(BlueprintType)
enum EMove_Way_Type : uint8
{
	Bird_View			UMETA(DisplayName="鸟瞰"),				//鸟瞰移动方式
	First_Person_View	UMETA(DisplayName="第一人称视角"),		//第一人称视角移动方式
	Third_Person_View	UMETA(DisplayName="第三人称视角"),		//第三人称视角移动方式
	None				UMETA(DisplayName="无移动方式"),			//无人物移动方式
};
