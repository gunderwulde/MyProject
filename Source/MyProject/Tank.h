// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"


// This struct covers all posiible tank unput schemes.
// What the inputs do can vary by tabk, but the same inouts will always exist.

USTRUCT(BlueprintType)
struct FTankInput
{
	GENERATED_BODY()

public:
	// Sanitized movement input, ready ti be used by game logiv
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input")
	FVector2D MovementInput;

	void Sanitize();
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

private:
	FVector2D RawMovementInput;

};

UCLASS()
class MYPROJECT_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveX(float AxisValue);
	void MoveY(float AxisValue);

private:
	// Helpful debug tool - witch way is the tasnk facing.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent*  TankDirection;

	//  Sprite for the tank body.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent*  TankSprite;

	// The actor used as the turret.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildTurret;

	// Our in-game camera.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tank Input", meta = (AllowPrivateAccess = "true"))
	FTankInput TankInput;


	// Maximun turn rate (degrees/second) of thank.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (ClampMin = "0.0"))
	float YawSpeed;

	// Maximun turn rate (unids/second) of thank.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tank", meta = (ClampMin = "0.0"))
	float MoveSpeed;

};
