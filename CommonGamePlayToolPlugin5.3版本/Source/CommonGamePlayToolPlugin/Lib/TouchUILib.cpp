// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchUILib.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"
#include "Components/PanelWidget.h"

#pragma region 获取UI大小信息
FVector2D UTouchUILib::GetWidgetAbsolutionSize(const UUserWidget* InWidget)
{
	return InWidget->GetPaintSpaceGeometry().GetAbsoluteSize();
}

FVector2D UTouchUILib::GetWidgetBaseSize(const UUserWidget* InWidget)
{
	return InWidget->GetPaintSpaceGeometry().GetLocalSize();
}

FVector2D UTouchUILib::GetWidgetDPISize(const UUserWidget* InWidget)
{
	return InWidget->GetPaintSpaceGeometry().GetLocalSize()*UWidgetLayoutLibrary::GetViewportScale(InWidget);
}

FVector2D UTouchUILib::GetWidgetShowSize(const UUserWidget* InWidget)
{
	return InWidget->GetPaintSpaceGeometry().GetLocalSize()*UWidgetLayoutLibrary::GetViewportScale(InWidget) * InWidget->GetRenderTransform().Scale;
}

FVector2D UTouchUILib::GetWidgetAbsolutePosition(const UUserWidget* InWidget)
{
	return InWidget->GetPaintSpaceGeometry().GetAbsolutePosition();
}

FVector2D UTouchUILib::GetWidgetInLocalCoordinatesLocation(const FVector2D InVector,const UUserWidget* InWidget)
{
	return  InWidget->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ InVector.X,InVector.Y });
}
#pragma endregion

