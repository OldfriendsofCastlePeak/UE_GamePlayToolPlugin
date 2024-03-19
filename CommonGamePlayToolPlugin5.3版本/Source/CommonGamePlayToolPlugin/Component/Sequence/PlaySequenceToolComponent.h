// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceCameraSettings.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Components/ActorComponent.h"
#include "PlaySequenceToolComponent.generated.h"



UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent),Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API UPlaySequenceToolComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlaySequenceToolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	/* 声明委托类型 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSequencePlayerDelegate,FName,Value);
	
	/* 当关卡序列开始播放时触发 */
	UPROPERTY(BlueprintAssignable,Category="PlaySequenceToolComponent|SequencePlayer")
	FSequencePlayerDelegate StartPlaySequenceEvent;

	/* 当关卡序列开始反向播放时触发 */
	UPROPERTY(BlueprintAssignable,Category="PlaySequenceToolComponent|SequencePlayer")
	FSequencePlayerDelegate StartReversePlaySequence;

	/* 当关卡序列退出时触发 */
	UPROPERTY(BlueprintAssignable,Category="PlaySequenceToolComponent|SequencePlayer")
	FSequencePlayerDelegate StopPlaySequence;

	/* 当关卡序列暂停时触发 */
	UPROPERTY(BlueprintAssignable,Category="PlaySequenceToolComponent|SequencePlayer")
	FSequencePlayerDelegate PausePlaySequence;

	/* 当关卡序列结束时触发 */
	UPROPERTY(BlueprintAssignable,Category="PlaySequenceToolComponent|SequencePlayer")
	FSequencePlayerDelegate FinshPlaySequence;

	/* 关卡序列播放设置 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlaySequenceToolComponent|SequencePlayer")
	FMovieSceneSequencePlaybackSettings SequencePlaybackSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Cameras", meta=(ShowOnlyInnerProperties, ExposeOnSpawn))
	FLevelSequenceCameraSettings CameraSettings;

	/* ULevelSequencePlayer对象,用于播放 ULevelSequence */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlaySequenceToolComponent")
	class ULevelSequencePlayer* SequencePlayer;

	/* 用来表示是否已经中断Sequence播放 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PlaySequenceToolComponent")
	bool bHad_Break=false;

	/* 上一次退出播放时处于的帧数,用于继续播放时使用 */
	UPROPERTY(BlueprintReadWrite,Category="PlaySequenceToolComponent")
	int32 LastStopPlayFrame=-1;

	/* 上一次退出播放时视角的位置 */
	UPROPERTY(BlueprintReadWrite,Category="PlaySequenceToolComponent")
	FVector LastViewLocation;

	/* 上一次退出播放时视角的方向 */
	UPROPERTY(BlueprintReadWrite,Category="PlaySequenceToolComponent")
	FRotator LastViewRotate;
	
	
	/* 中断播放关卡序列 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="PlaySequenceToolComponent")
	void StopPlayLevelSequence();
	
	/* 根据上一次中断位置继续播放关卡序列 */
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="PlaySequenceToolComponent")
	void ResumePlayLevelSequence();
	
	/* 播放关卡序列 */
	UFUNCTION(BlueprintCallable,Category="PlaySequenceToolComponent|SequencePlayer")
	void PlayLevelSequence(class ULevelSequence* InLevelSequenceAsset,const FName Name);

	/* 播放关卡序列 */
	UFUNCTION(BlueprintCallable,Category="PlaySequenceToolComponent|SequencePlayer")
	void PlayActorLevelSequence(class ALevelSequenceActor* InLevelSequenceAsset,const FName Name);

	/* 用于记录当前播放的Sequence的标识名,需要传入 */
	UPROPERTY(BlueprintReadWrite,Category="AAIBaseCharacter")
	FName CurrentPlayName;
	
	UFUNCTION()
	void OnSequencePlay_Internal();

	UFUNCTION()
	void OnSequenceReversePlay_Internal();
	
	UFUNCTION()
	void OnSequenceStop_Internal();
	
	UFUNCTION()
	void OnSequencePause_Internal();
	
	UFUNCTION()
	void OnSequenceFinalPlay_Internal();
};
