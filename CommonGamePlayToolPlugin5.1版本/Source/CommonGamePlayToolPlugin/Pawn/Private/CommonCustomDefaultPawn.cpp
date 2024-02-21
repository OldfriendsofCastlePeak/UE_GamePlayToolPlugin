// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonGamePlayToolPlugin/Pawn/Public/CommonCustomDefaultPawn.h"
#include "../../CommonGamePlayToolPlugin.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/SphereComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

// Sets default values
ACommonCustomDefaultPawn::ACommonCustomDefaultPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	NetPriority = 3.0f;

	BaseEyeHeight = 0.0f;
	bCollideWhenPlacing = false;
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(TEXT("PawnMovementComponent"));
	MovementComponent->UpdatedComponent = CollisionComponent;
	
	MeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Engine/EngineMeshes/Sphere")).Object);
		MeshComponent->AlwaysLoadOnClient = true;
		MeshComponent->AlwaysLoadOnServer = true;
		MeshComponent->bOwnerNoSee = true;
		MeshComponent->bCastDynamicShadow = true;
		MeshComponent->bAffectDynamicIndirectLighting = false;
		MeshComponent->bAffectDistanceFieldLighting = false;
		MeshComponent->bVisibleInRayTracing = false;
		MeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		const float Scale = CollisionComponent->GetUnscaledSphereRadius() / 160.f; // @TODO: hardcoding known size of EngineMeshes.Sphere. Should use a unit sphere instead.
		MeshComponent->SetRelativeScale3D(FVector(Scale));
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCanEverAffectNavigation(false);
	}
}

// Called when the game starts or when spawned
void ACommonCustomDefaultPawn::BeginPlay()
{
	Super::BeginPlay();
	if (CustomPawnInputMappingContext)
	{
		if (APlayerController* PC=Cast<APlayerController>(GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputSystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
			{
				EnhancedInputSystem->AddMappingContext(CustomPawnInputMappingContext,0);
			}
		}
	}
}

// Called every frame
void ACommonCustomDefaultPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ACommonCustomDefaultPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	RegisterInputAction(PlayerInputComponent);
}

void ACommonCustomDefaultPawn::RegisterInputAction(class UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent=Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//鼠标输入
		if (!MouseInputAction)
		{
			MouseInputAction=LoadObject<UInputAction>(this,TEXT("/Script/EnhancedInput.InputAction'/CommonGamePlayToolPlugin/Input/Input_Float2D/IA_Float2D_Mouse.IA_Float2D_Mouse'"));
			if (!MouseInputAction)
			{
				UE_LOG(FCommonGamePlayLog,Warning,TEXT("MouseInputAction is invalid"));
				
			}
			else
			{
				EnhancedInputComponent->BindAction(MouseInputAction,ETriggerEvent::Triggered,this,&ACommonCustomDefaultPawn::MouseInputEvent);
			}
		}

		//按键输入
		if (!MoveInputAction)
		{
			MoveInputAction=LoadObject<UInputAction>(this,TEXT("/Script/EnhancedInput.InputAction'/CommonGamePlayToolPlugin/Input/Input_Float2D/IA_Float2D_Move.IA_Float2D_Move'"));
			if (!MoveInputAction)
			{
				UE_LOG(FCommonGamePlayLog,Warning,TEXT("MoveInputAction is invalid"));
			}
			else
			{
				EnhancedInputComponent->BindAction(MoveInputAction,ETriggerEvent::Triggered,this,&ACommonCustomDefaultPawn::MoveInputEvent);
			}
		}

		
		if (!UpDownInputAction)
		{
			UpDownInputAction=LoadObject<UInputAction>(this,TEXT("/Script/EnhancedInput.InputAction'/CommonGamePlayToolPlugin/Input/Input_Float1D/IA_Float1D_Up.IA_Float1D_Up'"));
			if (!UpDownInputAction)
			{
				UE_LOG(FCommonGamePlayLog,Warning,TEXT("UpDownInputAction is invalid"));
			}
			else
			{
				EnhancedInputComponent->BindAction(UpDownInputAction,ETriggerEvent::Triggered,this,&ACommonCustomDefaultPawn::MoveUpInputEvent);
			}
		}
		
		//输入上下文,将在Beginplay中进行注册
		if (!CustomPawnInputMappingContext)
		{
			CustomPawnInputMappingContext=LoadObject<UInputMappingContext>(this,TEXT("/Script/EnhancedInput.InputMappingContext'/CommonGamePlayToolPlugin/Input/Game_Input_Context.Game_Input_Context'"));
			if (!CustomPawnInputMappingContext)
			{
				UE_LOG(FCommonGamePlayLog,Warning,TEXT("CustomPawnInputMappingContext is invalid"));
			}
		}
	}
	
}




void ACommonCustomDefaultPawn::MouseInputEvent_Implementation(const FInputActionInstance& InMouseInput)
{
	FVector2D Input2D=InMouseInput.GetValue().Get<FVector2D>();

	AddControllerYawInput(Input2D.X * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
	AddControllerPitchInput(Input2D.Y * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ACommonCustomDefaultPawn::MoveInputEvent_Implementation(const FInputActionInstance& MoveInput)
{
	FVector2D Input2D=MoveInput.GetValue().Get<FVector2D>();

	//FVector2D的第一个值表示前后移动,第二个值表示左右移动

	if (Input2D.X != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::X ), Input2D.X );
		}
	}
	
	if (Input2D.Y != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput( FRotationMatrix(ControlSpaceRot).GetScaledAxis( EAxis::Y ), Input2D.Y);
		}
	}
	
}


void ACommonCustomDefaultPawn::MoveUpInputEvent_Implementation(const FInputActionInstance& MoveUpInput)
{
	float Input1D=MoveUpInput.GetValue().Get<float>();
	if (Input1D != 0.f)
	{
		AddMovementInput(FVector::UpVector, Input1D);
	}
}