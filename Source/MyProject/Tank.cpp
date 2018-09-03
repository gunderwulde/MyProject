// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankStatics.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "PaperSpriteComponent.h"


void FTankInput::Sanitize() {
	MovementInput = RawMovementInput.ClampAxes(-1.0f, 1.0f);
	MovementInput = MovementInput.GetSafeNormal();

	RawMovementInput.Set(0.0f, 0.0f);

}

void FTankInput::MoveX(float AxisValue) {
	RawMovementInput.X = AxisValue;
}

void FTankInput::MoveY(float AxisValue) {
	RawMovementInput.Y = AxisValue;
}


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent) 
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
	}

	TankDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("TankDirection"));
	TankDirection->AttachTo(RootComponent);

	TankSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite"));
	TankSprite->AttachTo(TankDirection);

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 2.0f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->AttachTo(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 1024.0f;
	CameraComponent->AspectRatio = 3.0f / 4.0f;
	CameraComponent->AttachTo(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	ChildTurret = CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret"));
	ChildTurret->AttachTo(TankDirection);

}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TankInput.Sanitize();

	// Move the tank!
	{
		if (!TankInput.MovementInput.IsNearlyZero()) {
			FVector DesiredMovementDirection = FVector(-TankInput.MovementInput.Y, TankInput.MovementInput.X, 0.0f);
			// Rotate the tank! Note that we worate the TankDirection component,
			// not the RootComponent.
			FRotator MovementAngle = DesiredMovementDirection.Rotation();
			float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(TankDirection->GetComponentRotation().Yaw, MovementAngle.Yaw);
			bool bReverse = false;
			if (DeltaYaw != 0.0f) {
				float AdjustedDeltaYaw = DeltaYaw;
				if (AdjustedDeltaYaw < -90.0f) {
					AdjustedDeltaYaw += 180.0f;
					bReverse = true;
				}else if (AdjustedDeltaYaw > 90.0f) {
					AdjustedDeltaYaw -= 180.0f;
					bReverse = true;
				}

				// Turn toward the desired angle. Stop if we can get there in one frame.
				float MaxYawThisFrame = YawSpeed * DeltaTime;
				if (MaxYawThisFrame >= FMath::Abs(AdjustedDeltaYaw)) {
					if (bReverse) 
					{
						// Move backward.
						FRotator FacingAngle = MovementAngle;
						FacingAngle.Yaw = MovementAngle.Yaw + 180.0f;
						TankDirection->SetWorldRotation(FacingAngle);
					}
					else {
						TankDirection->SetWorldRotation(MovementAngle);
					}
				}
				else
				{
					// Can't reach our desired angle this frame, rotate part way.
					TankDirection->AddLocalRotation(FRotator(0.0f, FMath::Sign(AdjustedDeltaYaw) * MaxYawThisFrame, 0.0f));
				}

				// Move the tank!
				FVector MovementDirection = TankDirection->GetForwardVector() * (bReverse ? -1.0f : 1.0f);
				FVector Pos = GetActorLocation();
				Pos.X += MovementDirection.X *  MoveSpeed * DeltaTime;
				Pos.Y += MovementDirection.Y *  MoveSpeed * DeltaTime;
				SetActorLocation(Pos);
			}
		}
	}


}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveX"), this, &ATank::MoveX );
	PlayerInputComponent->BindAxis(TEXT("MoveY"), this, &ATank::MoveY );
}

void ATank::MoveX(float AxisValue) {
	TankInput.MoveX( AxisValue );
}

void ATank::MoveY(float AxisValue) {
	TankInput.MoveY(AxisValue);
}


