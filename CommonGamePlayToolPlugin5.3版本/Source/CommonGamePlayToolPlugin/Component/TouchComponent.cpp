// Fill out your copyright notice in the Description page of Project Settings.


#include "TouchComponent.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values for this component's properties
UTouchComponent::UTouchComponent()
{
	//Tick没必要开启
	PrimaryComponentTick.bCanEverTick = false;

	//创建对应的InputAction对象
	IA_Float2D_Touch1=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float2D_Touch1"));
	IA_Float2D_Touch1->ValueType=EInputActionValueType::Axis2D;

	IA_Float2D_Touch2=CreateDefaultSubobject<UInputAction>(TEXT("IA_Float2D_Touch2"));
	IA_Float2D_Touch2->ValueType=EInputActionValueType::Axis2D;

	//创建对应的UInputMappingContext对象
	Touch_Input_Mapping_Context=CreateDefaultSubobject<UInputMappingContext>(TEXT("Touch_Input_Mapping_Context"));
	












	
	
}


// Called when the game starts
void UTouchComponent::BeginPlay()
{
	Super::BeginPlay();

	//将UInputAction和对应的Touch触碰行为映射
	AddInputActionToInputMappingContext();

	//将输入映射注册到增强输入子系统中
	RegisterInputMappingContextToEnhanceInoput();
	
}


// Called every frame
void UTouchComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTouchComponent::Touch1StartedEvent_Implementation(const FInputActionInstance& InputValue)
{
	//Touch1开始触碰时将bTouch1Press设置为true
	bTouch1Press=true;

	//将Touch1Direction归零
	Touch1Direction.Zero();

	//将Touch1LastLocation设置为当前触碰位置
	Touch1LastLocation=InputValue.GetValue().Get<FVector2D>();
}

void UTouchComponent::Touch1TriggeredEvent_Implementation(const FInputActionInstance& InputValue)
{
	if (bTouch2Press)
	{
		//bTouch2Press为true时不做处理,在Touch2中的Triggered中进行处理
	}
	else
	{
		if (!Touch1Direction.IsZero())
		{
			//触碰1的向量不为0时调用委托
			if (OneFingerTouchMove.IsBound())
			{
				OneFingerTouchMove.Broadcast(Touch1Direction);
			}
		}
	}
	//计算Touch1最新的移动向量轨迹
	Touch1Direction=InputValue.GetValue().Get<FVector2D>()-Touch1LastLocation;

	//更新位置
	Touch1LastLocation=InputValue.GetValue().Get<FVector2D>();
}

void UTouchComponent::Touch1CompletedEvent_Implementation(const FInputActionInstance& InputValue)
{
	//Touch1结束触碰时将bTouch1Press设置为false
	bTouch1Press=false;

	//将Touch1Direction归零
	Touch1Direction.Zero();

	//将Touch1LastLocation归零
	Touch1LastLocation.Zero();
}

void UTouchComponent::Touch2StartedEvent_Implementation(const FInputActionInstance& InputValue)
{
	//Touch1开始触碰时将bTouch1Press设置为true
	bTouch2Press=true;

	//将Touch1Direction归零
	Touch2Direction.Zero();

	//将Touch1LastLocation设置为当前触碰位置
	Touch2LastLocation=InputValue.GetValue().Get<FVector2D>();
}

void UTouchComponent::Touch2TriggeredEvent_Implementation(const FInputActionInstance& InputValue)
{
	if (bTouch1Press)
	{
		//处理双指行为
		if (Touch1Direction.IsZero())
		{
			//Touch1Direction为0,Touch2Direction不为0则广播委托
			if (!Touch2Direction.IsZero())
			{
				if (OneFingerTouchMove.IsBound())
				{
					OneFingerTouchMove.Broadcast(Touch2Direction);
				}
			}
		}
		else
		{
			if (Touch2Direction.IsZero())
			{
				//Touch2Direction为0,则使用Touch1Direction的值广播委托
				if (OneFingerTouchMove.IsBound())
				{
					OneFingerTouchMove.Broadcast(Touch1Direction);
				}
			}
			else
			{
				//使用点乘+反余弦获取两个向量的夹角
				if (FMath::TruncToInt(UKismetMathLibrary::DegAcos(FVector2D::DotProduct(Touch1Direction,Touch2Direction)))==0)
				{
					//两个向量间的夹角为0,两个向量任选其一传入
					if (OneFingerTouchMove.IsBound())
					{
						OneFingerTouchMove.Broadcast(Touch1Direction);
					}
				}
				else
				{
					if (TwoFingerTouchScale.IsBound())
					{
						const FVector2D Touch2CurrentLocation=InputValue.GetValue().Get<FVector2D>();
						//(Touch2CurrentLocation-Touch1LastLocation).Length()为当前的Touch2点到Tocuh1点的向量长度
						//(Touch2LastLocation-Touch1LastLocation).Length()为前一次的Touch2点到Tocuh1点的向量长度

						//单位符号,用于确定是缩小还是放大
						const int32 UnitSign=(Touch2CurrentLocation-Touch1LastLocation).Length()>(Touch2LastLocation-Touch1LastLocation).Length()?1:-1;

						//广播双指操作
						TwoFingerTouchScale.Broadcast(FMath::Abs((Touch2CurrentLocation-Touch2LastLocation).Length())*UnitSign);
					}
				}
			}
		}
	}
	else
	{
		if (!Touch2Direction.IsZero())
		{
			//触碰1的向量不为0时调用委托
			if (OneFingerTouchMove.IsBound())
			{
				OneFingerTouchMove.Broadcast(Touch2Direction);
			}
		}
	}

	//计算Touch1最新的移动向量轨迹
	Touch2Direction=InputValue.GetValue().Get<FVector2D>()-Touch2LastLocation;

	//更新位置
	Touch2LastLocation=InputValue.GetValue().Get<FVector2D>();
}

void UTouchComponent::Touch2CompletedEvent_Implementation(const FInputActionInstance& InputValue)
{
	//Touch1结束触碰时将bTouch1Press设置为false
	bTouch2Press=false;

	//将Touch1Direction归零
	Touch2Direction.Zero();

	//将Touch1LastLocation归零
	Touch2LastLocation.Zero();
}

void UTouchComponent::AddInputActionToInputMappingContext()
{
	Touch_Input_Mapping_Context->MapKey(IA_Float2D_Touch1,EKeys::TouchKeys[0]);
	Touch_Input_Mapping_Context->MapKey(IA_Float2D_Touch2,EKeys::TouchKeys[1]);

	if (UInputComponent* InputCom=this->GetOwner()->InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputCom=Cast<UEnhancedInputComponent>(InputCom))
		{
			EnhancedInputCom->BindAction(IA_Float2D_Touch1,ETriggerEvent::Started,this,&UTouchComponent::Touch1StartedEvent);
			EnhancedInputCom->BindAction(IA_Float2D_Touch1,ETriggerEvent::Triggered,this,&UTouchComponent::Touch1TriggeredEvent);
			EnhancedInputCom->BindAction(IA_Float2D_Touch1,ETriggerEvent::Completed,this,&UTouchComponent::Touch1CompletedEvent);

			EnhancedInputCom->BindAction(IA_Float2D_Touch2,ETriggerEvent::Started,this,&UTouchComponent::Touch2StartedEvent);
			EnhancedInputCom->BindAction(IA_Float2D_Touch2,ETriggerEvent::Triggered,this,&UTouchComponent::Touch2TriggeredEvent);
			EnhancedInputCom->BindAction(IA_Float2D_Touch2,ETriggerEvent::Completed,this,&UTouchComponent::Touch2CompletedEvent);
		}

		
	}

	
	
}

void UTouchComponent::RegisterInputMappingContextToEnhanceInoput() const
{
	//获取该组件Owner的控制器
	if (APlayerController* PC=Cast<APlayerController>(Cast<APawn>(this->GetOwner())->GetController()))
	{
		//通过控制器获取 增强输入 本地玩家的子系统
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(Touch_Input_Mapping_Context,InputPriority);
		}
	}
}

