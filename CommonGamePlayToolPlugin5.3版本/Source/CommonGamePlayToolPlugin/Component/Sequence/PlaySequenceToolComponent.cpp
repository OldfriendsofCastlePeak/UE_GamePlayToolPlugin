// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaySequenceToolComponent.h"
#include "Runtime/LevelSequence/Public/LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"


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

void UPlaySequenceToolComponent::StopPlayLevelSequence_Implementation()
{
	//先检查SequencePlayer是否处于无效状态
	if (!SequencePlayer) return;

	//先检查SequencePlayer是否处于播放状态
	if (!SequencePlayer->IsPlaying()) return;

	//记录当前的时间、视野位置、视野角度
	LastStopPlayFrame=SequencePlayer->GetCurrentTime().Time.GetFrame().Value;
	LastViewLocation=UGameplayStatics::GetPlayerCameraManager(this,0)->GetCameraLocation();
	LastViewRotate=UGameplayStatics::GetPlayerCameraManager(this,0)->GetCameraRotation();
	bHad_Break=true;
	
	//退出播放
	SequencePlayer->Stop();
}

void UPlaySequenceToolComponent::ResumePlayLevelSequence_Implementation()
{
	SequencePlayer->SetPlaybackPosition(FMovieSceneSequencePlaybackParams(FFrameTime(LastStopPlayFrame),EUpdatePositionMethod::Play));
	bHad_Break=false;
}

void UPlaySequenceToolComponent::PlayLevelSequence(ULevelSequence* InLevelSequenceAsset,const FName Name)
{
	//检测资源是否有效
	if (!InLevelSequenceAsset) return;

	CurrentPlayName=Name;
	
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

void UPlaySequenceToolComponent::PlayActorLevelSequence(ALevelSequenceActor* InLevelSequenceActor,const FName Name)
{
	//检查传入的ALevelSequenceActor是否有效.
	if (InLevelSequenceActor==nullptr) return;

	CurrentPlayName=Name;
	
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
	StartPlaySequenceEvent.Broadcast(CurrentPlayName);
}

void UPlaySequenceToolComponent::OnSequenceReversePlay_Internal()
{
	StartReversePlaySequence.Broadcast(CurrentPlayName);
}

void UPlaySequenceToolComponent::OnSequenceStop_Internal()
{
	StopPlaySequence.Broadcast(CurrentPlayName);
}

void UPlaySequenceToolComponent::OnSequencePause_Internal()
{
	PausePlaySequence.Broadcast(CurrentPlayName);
}

void UPlaySequenceToolComponent::OnSequenceFinalPlay_Internal()
{
	FinshPlaySequence.Broadcast(CurrentPlayName);
}



