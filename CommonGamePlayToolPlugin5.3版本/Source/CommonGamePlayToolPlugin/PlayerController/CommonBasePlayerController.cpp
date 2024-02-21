// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonBasePlayerController.h"


// Sets default values
ACommonBasePlayerController::ACommonBasePlayerController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACommonBasePlayerController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonBasePlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

