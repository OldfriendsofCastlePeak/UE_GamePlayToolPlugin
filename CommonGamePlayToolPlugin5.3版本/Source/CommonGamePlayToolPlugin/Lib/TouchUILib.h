// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Blueprint/UserWidget.h"
#include "TouchUILib.generated.h"

/**
 * 
 */
UCLASS()
class COMMONGAMEPLAYTOOLPLUGIN_API UTouchUILib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
#pragma region 获取UI大小信息

	/**
	 * 获取控件基本大小(设计中的大小).
	 * @return 返回控件基本大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	static FVector2D GetWidgetAbsolutionSize(const UUserWidget* InWidget);
	
	/**
	 * 获取控件基本大小(设计中的大小).
	 * @return 返回控件基本大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	static FVector2D GetWidgetBaseSize(const UUserWidget* InWidget);

	/**
	 * 获取控件受DPI影响的大小,例如:DPI设置的1080的DPI为1,则1440的DPI为1.333333,此时大小则为设计中的UI大小*1.333333
	 * @return 返回控件的显示大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	static FVector2D GetWidgetDPISize(const UUserWidget* InWidget);
	
	/**
	 * 获取控件的显示大小,计算公式为:UI设计大小*DPI*UI的Tansform的Scale属性
	 * @return 返回控件的显示大小
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Size")
	static FVector2D GetWidgetShowSize(const UUserWidget* InWidget);
	
#pragma endregion


#pragma region 获取UI位置信息
	/**/
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	static FVector2D GetWidgetAbsolutePosition(const UUserWidget* InWidget);
	
	/**
	 * 获取UI在本地空间中的位置,测试发现锚点在左上(0,0)时,返回的结果为UI的位置加其对应的宽高,
	 * 例如:位置为X:100,Y:200,宽为100,高为100,传入X:0.5,Y:0返回X:100+100*0.5,Y:200+100*0
	 * @param InVector
	 * @param InWidget 
	 * @return 返回UI在本地空间中的位置
	 */
	UFUNCTION(BlueprintPure, Category = "UTouchBaseWidget|Location")
	static FVector2D GetWidgetInLocalCoordinatesLocation(const FVector2D InVector,const UUserWidget* InWidget);
#pragma endregion




	
};
