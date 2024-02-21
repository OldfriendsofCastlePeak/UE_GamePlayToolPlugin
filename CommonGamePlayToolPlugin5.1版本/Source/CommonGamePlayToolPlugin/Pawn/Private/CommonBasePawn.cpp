// Fill out your copyright notice in the Description page of Project Settings.


#include "../Public/CommonBasePawn.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACommonBasePawn::ACommonBasePawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACommonBasePawn::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerCameraManager(this,0)->OnBlendComplete().AddLambda(
		[&]()
		{
			if (ViewBlendCompleteDeletate.IsBound())
			{
				ViewBlendCompleteDeletate.Broadcast();
			}
		}
	);



	

	
}

// Called every frame
void ACommonBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACommonBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}






