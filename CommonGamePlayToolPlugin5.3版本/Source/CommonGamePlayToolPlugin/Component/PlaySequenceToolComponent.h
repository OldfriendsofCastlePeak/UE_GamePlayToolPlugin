// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequenceCameraSettings.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Components/ActorComponent.h"
#include "PlaySequenceToolComponent.generated.h"

/* 声明委托类型 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSequencePlayerDelegate);

UCLASS(ClassGroup=(CommonToolComponent), meta=(BlueprintSpawnableComponent))
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
	
	/* 播放关卡序列 */
	UFUNCTION(BlueprintCallable,Category="PlaySequenceToolComponent|SequencePlayer")
	void PlayLevelSequence(class ULevelSequence* InLevelSequenceAsset);

	/* 播放关卡序列 */
	UFUNCTION(BlueprintCallable,Category="PlaySequenceToolComponent|SequencePlayer")
	void PlayActorLevelSequence(class ALevelSequenceActor* InLevelSequenceAsset);
	
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
