// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FPSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FPSCamera"));
	FPSCamera->SetupAttachment(GetCapsuleComponent());
	FPSCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 80.0f));

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetupAttachment(FPSCamera);
	Mesh1P->SetCastShadow(false);
	Mesh1P->SetOnlyOwnerSee(true);


	if (Mesh1P)
	{
		GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
		GunMesh->SetupAttachment(Mesh1P, TEXT("GripPoint"));
		GunMesh->SetCastShadow(false);
		GunMesh->SetOnlyOwnerSee(true);
	}

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FPSCamera->bUsePawnControlRotation = true;
	ACharacter::bUseControllerRotationYaw = false;
	ACharacter::bUseControllerRotationPitch = false;

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))  //조건식 내부 선언 방식 , 스코프 오염 방지
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (CrosshairWidgetClass != nullptr)
	{
		CrosshairWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
		if (CrosshairWidgetInstance != nullptr)
		{
			CrosshairWidgetInstance->AddToViewport();
		}
	}
	if (CombatComp)
	{
		CombatComp->CurrentWeaponMesh = GunMesh;
	}
}


// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look); //지속성 프레임호출
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyCharacter::Fire); //단발성

	}
}

void AMyCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMyCharacter:: Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		float YawInput = LookAxisVector.X * MouseSensitivity;
		float PitchInput = LookAxisVector.Y * MouseSensitivity;

		if (bInvertMouseY)
		{
			PitchInput *= -1.0f;
		}
		AddControllerYawInput(YawInput);
		AddControllerPitchInput(PitchInput);
	}
}
void AMyCharacter::Fire(const FInputActionValue& Value)
{
	//UE_LOG(LogTemp, Log, TEXT("Fire"));
	if (CombatComp)
	{
		CombatComp->FireTarget();
	}
}

