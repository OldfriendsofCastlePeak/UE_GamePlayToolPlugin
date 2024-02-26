// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySequenceToolComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequencePlayer.h"


// Sets default values for this component's properties
UPlaySequenceToolComponent::UPlaySequenceToolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlaySequenceToolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlaySequenceToolComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                               FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlaySequenceToolComponent::PlayLevelSequence(ULevelSequence* InLevelSequenceAsset)
{
	//检测资源是否有效
	if (!InLevelSequenceAsset)
	{
		GEngine->AddOnScreenDebugMessage(-1,2.f,FColor::White,TEXT("播放关卡序列失败,因为传入的ULevelSequence无效"));
		return;
	}
	
	
	if (!SequencePlayer)
	{
		ALevelSequenceActor* Tem=NewObject<ALevelSequenceActor>();
		
		SequencePlayer=ULevelSequencePlayer::CreateLevelSequencePlayer(this,InLevelSequenceAsset,SequencePlaybackSettings,Tem);

		
		SequencePlayer->OnPlay.AddDynamic(this,&UPlaySequenceToolComponent::OnSequencePlay_Internal);
		SequencePlayer->OnPlayReverse.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceReversePlay_Internal);
		SequencePlayer->OnStop.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceStop_Internal);
		SequencePlayer->OnPause.AddDynamic(this,&UPlaySequenceToolComponent::OnSequencePause_Internal);
		SequencePlayer->OnFinished.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceFinalPlay_Internal);
	}

	//检测要播放的关卡序列是否和当前的一致
	if (InLevelSequenceAsset==SequencePlayer->GetSequence())
	{
		
	}
	else
	{
		if (SequencePlayer->IsPlaying())
		{
			//停止播放动画序列
			SequencePlayer->Stop();
		}
		//更新要播放的Sequence资源
		SequencePlayer->Initialize(InLevelSequenceAsset,GetComponentLevel(),CameraSettings);
	}
	SequencePlayer->Play();
}

void UPlaySequenceToolComponent::PlayActorLevelSequence(ALevelSequenceActor* InLevelSequenceActor)
{
	//检查传入的ALevelSequenceActor是否有效.
	if (InLevelSequenceActor==nullptr) return;

	//获取有效的ULevelSequencePlayer
	if (!SequencePlayer)
	{
		ALevelSequenceActor* Tem=NewObject<ALevelSequenceActor>();
		SequencePlayer=ULevelSequencePlayer::CreateLevelSequencePlayer(this,InLevelSequenceActor->LevelSequenceAsset,SequencePlaybackSettings,Tem);

		
		SequencePlayer->OnPlay.AddDynamic(this,&UPlaySequenceToolComponent::OnSequencePlay_Internal);
		SequencePlayer->OnPlayReverse.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceReversePlay_Internal);
		SequencePlayer->OnStop.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceStop_Internal);
		SequencePlayer->OnPause.AddDynamic(this,&UPlaySequenceToolComponent::OnSequencePause_Internal);
		SequencePlayer->OnFinished.AddDynamic(this,&UPlaySequenceToolComponent::OnSequenceFinalPlay_Internal);
	}

	//检测要播放的关卡序列是否和当前的一致
	if (InLevelSequenceActor->LevelSequenceAsset==SequencePlayer->GetSequence())
	{
		
	}
	else
	{
		if (SequencePlayer->IsPlaying())
		{
			//停止播放动画序列
			SequencePlayer->Stop();
		}
		//更新要播放的Sequence资源
		SequencePlayer->Initialize(InLevelSequenceActor->LevelSequenceAsset,GetComponentLevel(),CameraSettings);
	}
	SequencePlayer->Play();
}

void UPlaySequenceToolComponent::OnSequencePlay_Internal()
{
	StartPlaySequenceEvent.Broadcast();
}

void UPlaySequenceToolComponent::OnSequenceReversePlay_Internal()
{
	StartReversePlaySequence.Broadcast();
}

void UPlaySequenceToolComponent::OnSequenceStop_Internal()
{
	StopPlaySequence.Broadcast();
}

void UPlaySequenceToolComponent::OnSequencePause_Internal()
{
	PausePlaySequence.Broadcast();
}

void UPlaySequenceToolComponent::OnSequenceFinalPlay_Internal()
{
	FinshPlaySequence.Broadcast();
}



