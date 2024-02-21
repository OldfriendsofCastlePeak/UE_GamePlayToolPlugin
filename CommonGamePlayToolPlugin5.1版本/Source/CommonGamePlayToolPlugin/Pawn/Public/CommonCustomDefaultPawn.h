// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonBasePawn.h"
#include "InputAction.h"
#include "Components/SphereComponent.h"
#include "CommonCustomDefaultPawn.generated.h"

UCLASS(Blueprintable,BlueprintType)
class COMMONGAMEPLAYTOOLPLUGIN_API ACommonCustomDefaultPawn : public ACommonBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACommonCustomDefaultPawn(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USphereComponent> CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	
	
	//转动X轴视野速度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pawn")
	float BaseTurnRate=45.0f;

	//转动Y轴视野速度
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pawn")
	float BaseLookUpRate=45.0f;




	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName="MouseInputAction")
	UInputAction* MouseInputAction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName="KeyboardInputAction")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName="KeyboardInputAction")
	UInputAction* UpDownInputAction;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,DisplayName="CustomPawnInputMappingContext")
	UInputMappingContext* CustomPawnInputMappingContext;

	void RegisterInputAction(class UInputComponent* PlayerInputComponent);

	UFUNCTION(BlueprintNativeEvent)
	void MouseInputEvent(const FInputActionInstance& InMouseInput);

	UFUNCTION(BlueprintNativeEvent)
	void MoveInputEvent(const FInputActionInstance& MoveInput);

	UFUNCTION(BlueprintNativeEvent)
	void MoveUpInputEvent(const FInputActionInstance& MoveUpInput);
	
};
