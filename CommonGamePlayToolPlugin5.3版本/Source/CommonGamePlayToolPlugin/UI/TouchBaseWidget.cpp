// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchBaseWidget.h"
#include "Components/PanelWidget.h"
#include "../Component/Touch/TouchComponent.h"

#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"


void UTouchBaseWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTouchBaseWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTouchBaseWidget::NativeOnInitialized()
{
	
	Super::NativeOnInitialized();

	//当bCustomTrigger为false时,将绑定默认的委托
	//if (!bCustomTrigger) BindTouchDelegate();
	
}
#pragma region 获取UI位置信息
FVector2D UTouchBaseWidget::GetLocalPosition()
{
	//获取UI在本地空间的左上位置
	FVector2D Offset =GetPaintSpaceGeometry().GetLocalPositionAtCoordinates({ 0.0,0.0 });

	//获取父UI
	const UWidget* TemParentWidget =GetParent();

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
	//检测UI是否有一个有效的APlayerController
	if (GetOwningPlayer())
	{
		//获取APlayerController控制的APawn
		UActorComponent* ActorComponent = GetOwningPlayer()->GetPawn()->GetComponentByClass(UTouchComponent::StaticClass());
		if (ActorComponent)
		{
			//获取APawn上的UTouchComponent
			UTouchComponent* TouchComponent = Cast<UTouchComponent>(ActorComponent);
			if (TouchComponent)
			{
				// //绑定触碰按下委托
				// FScriptDelegate Pressed_Delegate;
				// Pressed_Delegate.BindUFunction(this,"Touch_Pressed_Internal");
				// TouchComponent->On_Touch_Released.Add(Pressed_Delegate);
				//
				// //绑定触碰移动委托
				// FScriptDelegate Moved_Delegate;
				// Pressed_Delegate.BindUFunction(this,"Touch_Moved_Internal");
				// TouchComponent->On_Touch_Released.Add(Pressed_Delegate);
				//
				// //绑定触碰松开委托
				// FScriptDelegate Released_Delegate;
				// Pressed_Delegate.BindUFunction(this,"Touch_Released_Internal");
				// TouchComponent->On_Touch_Released.Add(Pressed_Delegate);
				//
				// //绑定到触碰组件灭活时调用
				// FScriptDelegate ScriptDelegate;
				// ScriptDelegate.BindUFunction(this, "ComponentDeactivated"); 
				// TouchComponent->OnComponentDeactivated.Add(ScriptDelegate);
			}
		}
	}
}

void UTouchBaseWidget::Touch_Pressed_Internal(const FVector& Location, uint8 FingerIndex)
{
	
}

void UTouchBaseWidget::Touch_Moved_Internal(const FVector& Location, uint8 FingerIndex)
{
	
}

void UTouchBaseWidget::Touch_Released_Internal(const FVector& Location, uint8 FingerIndex)
{
	
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
	const FVector2D UI_Size = UTouchUILib::GetWidgetShowSize(this);

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


void UTouchBaseWidget::TriggerInedxAnimation(int Index)
{
	BPTriggerInedxAnimation(Index);
}

void UTouchBaseWidget::ComponentDeactivated(UActorComponent* ActorComponent)
{
	BindTouchDelegate();
}

