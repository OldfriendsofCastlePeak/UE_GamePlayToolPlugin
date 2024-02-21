// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "CommonBaseGameState.generated.h"

UCLASS()
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonBaseGameState : public AGameState
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACommonBaseGameState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
