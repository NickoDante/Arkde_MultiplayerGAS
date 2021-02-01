// Copyright Epic Games, Inc. All Rights Reserved.

#include "Arkde_MultiplayerGASCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS_AttributeSet.h"
#include "GAS_GameplayAbility.h"
#include "Arkde_MultiplayerGAS/Arkde_MultiplayerGAS.h"

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

AArkde_MultiplayerGASCharacter::AArkde_MultiplayerGASCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AttributeSet = CreateDefaultSubobject<UGAS_AttributeSet>(TEXT("AttributeSet"));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(AbilitySystemComponent))
	{
		for (TSubclassOf<UGAS_GameplayAbility>& CurrentAbilityClass : StartingAbilities)
		{
			if (!IsValid(CurrentAbilityClass))
			{
				continue;
			}

			UGAS_GameplayAbility* CurrentAbility = CurrentAbilityClass->GetDefaultObject<UGAS_GameplayAbility>();
			if (!IsValid(CurrentAbility))
			{
				continue;
			}

			FGameplayAbilitySpec abilitySpec(CurrentAbility, 1, static_cast<int32>(CurrentAbility->AbilityInputID), this);
			AbilitySystemComponent->GiveAbility(abilitySpec);
		}

		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (IsValid(AbilitySystemComponent))
	{
		AbilitySystemComponent->RefreshAbilityActorInfo();
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArkde_MultiplayerGASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArkde_MultiplayerGASCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArkde_MultiplayerGASCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArkde_MultiplayerGASCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AArkde_MultiplayerGASCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AArkde_MultiplayerGASCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AArkde_MultiplayerGASCharacter::OnResetVR);

	// Setup Ability System Component Input Bindings
	AbilitySystemComponent->BindAbilityActivationToInputComponent(
		PlayerInputComponent,
		FGameplayAbilityInputBinds(
			"Confirm",
			"Cancel",
			"EGAS_AbilityInputID",
			static_cast<int32>(EGAS_AbilityInputID::InputID_Confirm),
			static_cast<int32>(EGAS_AbilityInputID::InputID_Cancel)
		)
	);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

UAbilitySystemComponent* AArkde_MultiplayerGASCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void AArkde_MultiplayerGASCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
