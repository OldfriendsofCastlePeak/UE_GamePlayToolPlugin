// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewBlendComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UViewBlendComponent::UViewBlendComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UViewBlendComponent::BeginPlay()
{
	Super::BeginPlay();

	//初始化进行绑定
	BindDelegate();











	
}


// Called every frame
void UViewBlendComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UViewBlendComponent::UpdateCurrentPlayerIndex()
{
	if (GetOwner()->IsA(APawn::StaticClass()))
	{
		const APawn* OwnerPawn=Cast<APawn>(GetOwner());
		//进行检测
		if (!OwnerPawn) return;
		
		if (OwnerPawn->IsPlayerControlled())
		{
			//不等则需要对委托进行解绑,再重新绑定
			if (CurrentPlayerIndex!=UGameplayStatics::GetPlayerControllerID(OwnerPawn->GetController<APlayerController>()))
			{
				CurrentPlayerIndex=UGameplayStatics::GetPlayerControllerID(OwnerPawn->GetController<APlayerController>());
				//先解绑
				UnBindDelegate();
				//再重新绑定
				BindDelegate();
			}
		}
	}
}

int UViewBlendComponent::GetCurrentPlayerIndex()const
{
	return CurrentPlayerIndex;
}

void UViewBlendComponent::BindDelegate()
{
	UGameplayStatics::GetPlayerCameraManager(this,CurrentPlayerIndex)->OnBlendComplete().AddLambda([this]()
	{
		this->OnEndViewBlend.Broadcast();	
	});
}

void UViewBlendComponent::UnBindDelegate()
{
	//待编写...
}

