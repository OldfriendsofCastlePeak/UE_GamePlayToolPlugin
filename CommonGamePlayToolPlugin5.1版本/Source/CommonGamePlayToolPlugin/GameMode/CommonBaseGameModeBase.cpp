// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonBaseGameModeBase.h"


// Sets default values
ACommonBaseGameModeBase::ACommonBaseGameModeBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACommonBaseGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACommonBaseGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

