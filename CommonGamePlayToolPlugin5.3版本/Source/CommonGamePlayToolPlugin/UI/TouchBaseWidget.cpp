// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchBaseWidget.h"
#include "Components/PanelWidget.h"
#include "../Component/TouchComponent.h"
#include "Kismet/KismetMathLibrary.h" //官方函数库
#include "Runtime/Engine/Public/DelayAction.h" //延迟的函数库

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"


void UTouchBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
	SetDisabled(bDisabled);
}

void UTouchBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void UTouchBaseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (bCustomTrigger == false)
	{
		BindTouchDelegate();
	}
	
}


#pragma region 获取UI大小信息
FVector2D UTouchBaseWidget::GetWidgetAbsolutionSize()
{
	return GetPaintSpaceGeometry().GetAbsoluteSize();
}

FVector2D UTouchBaseWidget::GetWidgetBaseSize()
{
	return GetPaintSpaceGeometry().GetLocalSize();
}

FVector2D UTouchBaseWidget::GetWidgetDPISize()
{
	return GetPaintSpaceGeometry().GetLocalSize()*UWidgetLayoutLibrary::GetViewportScale(this);
}

FVector2D UTouchBaseWidget::GetWidgetShowSize()
{
	return GetPaintSpaceGeometry().GetLocalSize()*UWidgetLayoutLibrary::GetViewportScale(this) * GetRenderTransform().Scale;
}

FVector2D UTouchBaseWidget::GetWidgetAbsolutePosition()
{
	return GetPaintSpaceGeometry().GetAbsolutePosition();
}

FVector2D UTouchBaseWidget::GetWidgetInLocalCoordinatesLocation(const FVector2D InVector)
{
	return  GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ InVector.X,InVector.Y });
}
#pragma endregion

#pragma region 获取UI位置信息
FVector2D UTouchBaseWidget::GetLocalPosition()
{
	//获取UI在本地空间的左上位置
	FVector2D Offset = GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });

	//获取父UI
	const UWidget* TemParentWidget = GetParent();

	//
	while (TemParentWidget)
	{
		Offset += TemParentWidget->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });
		TemParentWidget = TemParentWidget->GetParent();
	}
	
	if (ParentWidget)
	{
		TemParentWidget = ParentWidget;
		while (TemParentWidget)
		{
			Offset += TemParentWidget->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });
			TemParentWidget = TemParentWidget->GetParent();
		}
	}
	
	return Offset + CustomOffsetPosition;
}

#pragma endregion
void UTouchBaseWidget::BindTouchDelegate()
{
	if (GetOwningPlayer())
	{
		UActorComponent* ActorComponent = GetOwningPlayer()->GetComponentByClass(UTouchComponent::StaticClass());
		if (ActorComponent)
		{
			UTouchComponent* TouchComponent = Cast<UTouchComponent>(ActorComponent);
			if (TouchComponent)
			{
				//TouchComponent->DelegateBind(10, true, this, "TouchIndexLocation");
				
				//绑定函数的触碰组件灭活时调用
				FScriptDelegate ScriptDelegate;
				ScriptDelegate.BindUFunction(this, "ComponentDeactivated"); 
				TouchComponent->OnComponentDeactivated.Add(ScriptDelegate);
				return;
			}
		}
	}
	if(GetWorld())
	{
		//这一步可能无意义
		// FLatentActionManager& LatentActionManager = GetWorld()->GetLatentActionManager();
		// FLatentActionInfo Latentinfo;
		// Latentinfo.CallbackTarget = this;
		// Latentinfo.ExecutionFunction = "BindTouchDelegate";
		// Latentinfo.Linkage = 0;
		// Latentinfo.UUID = UKismetMathLibrary::RandomIntegerInRange(0, 222);
		// LatentActionManager.AddNewAction(this, Latentinfo.UUID, new FDelayAction(0.2, Latentinfo));
	}
}


void UTouchBaseWidget::TouchIndexLocation(const FVector& Location, uint8 FingerIndex)
{
	if (IsTouchLocation(Location))
	{
		LastTriggerLocation = Location;
		LastTriggerLocation.Z = FingerIndex;

		//广播触碰位置
		OnTouchLocation.Broadcast(LastTriggerLocation);
		
		TriggerInedxAnimation(0);
	}
}

void UTouchBaseWidget::SetIndexTouchDelegate(bool bDelegateBind, uint8 FingerIndex)
{
	UTouchComponent* TouchComponent = Cast<UTouchComponent>(GetOwningPlayer()->GetComponentByClass(UTouchComponent::StaticClass()));
	if (TouchComponent)
	{
		//TouchComponent->DelegateBind(FingerIndex, bDelegateBind, this, "TouchMovedLocation");
	}
}

void UTouchBaseWidget::TouchMovedLocation(const FVector& Location)
{
	LastTriggerLocation = Location;
	/** * 子类继承重写使用 */
}



bool UTouchBaseWidget::IsTouchLocation(const FVector& Location)
{
	//获取应用于视口和所有小部件的当前DPI比例.
	const float Viewport_Scale = UWidgetLayoutLibrary::GetViewportScale(this);
	
	//获取控件大小
	const FVector2D UI_Size = GetWidgetShowSize();

	//获取控件左上角位置
	LocalWidgetPosition = GetLocalPosition();

	//
	TriggerOffsetPosition = FVector2D(Location) / Viewport_Scale - LocalWidgetPosition;

	//计算缩放偏移
	const FVector2D TLocalWidgetPosition = LocalWidgetPosition * Viewport_Scale - UI_Size / 4 * (GetRenderTransform().Scale - 1);

	//检查位置Location.X是否在范围内
	const bool bX_Inrange=Location.X >= TLocalWidgetPosition.X && Location.X <= TLocalWidgetPosition.X + UI_Size.X;
	
	//检查位置Location.Y是否在范围内
	const bool bY_Inrange=Location.Y >= TLocalWidgetPosition.Y && Location.Y <= TLocalWidgetPosition.Y + UI_Size.Y;
	
	return bX_Inrange&&bY_Inrange;
}

void UTouchBaseWidget::SetDisabled(bool bIsDisabled)
{
	bDisabled = bIsDisabled;
}

void UTouchBaseWidget::TriggerInedxAnimation(int Index)
{
	BPTriggerInedxAnimation(Index);
}

void UTouchBaseWidget::ComponentDeactivated(UActorComponent* ActorComponent)
{
	
	BindTouchDelegate();
}

