// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieProjectPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "ZombieProjectCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AZombieProjectPlayerController::AZombieProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	CachedDestination = FVector::ZeroVector;
	FollowTime = 0.f;
}

void AZombieProjectPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AZombieProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Setup mouse input events
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Started, this, &AZombieProjectPlayerController::OnInputStarted);
		/*
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Triggered, this, &AZombieProjectPlayerController::OnSetDestinationTriggered);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Completed, this, &AZombieProjectPlayerController::OnSetDestinationReleased);
		EnhancedInputComponent->BindAction(SetDestinationClickAction, ETriggerEvent::Canceled, this, &AZombieProjectPlayerController::OnSetDestinationReleased);*/
		EnhancedInputComponent->BindAction(MoveCameraF, ETriggerEvent::Triggered, this, &AZombieProjectPlayerController::OnMoveForward);
		EnhancedInputComponent->BindAction(MoveCameraB, ETriggerEvent::Triggered, this, &AZombieProjectPlayerController::OnMoveBackward);
		EnhancedInputComponent->BindAction(MoveCameraL, ETriggerEvent::Triggered, this, &AZombieProjectPlayerController::OnMoveLeft);
		EnhancedInputComponent->BindAction(MoveCameraR, ETriggerEvent::Triggered, this, &AZombieProjectPlayerController::OnMoveRight);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AZombieProjectPlayerController::OnInputStarted()
{
	FHitResult Hit;
	bool bHitSuccessful = false;
	bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	if (bHitSuccessful) {
		CachedDestination = Hit.Location;
	}
	if (ActorToSpawn) {
		FVector Destination = CachedDestination;
		FRotator SpawnRotation = FRotator::ZeroRotator;
		GetWorld()->SpawnActor<AActor>(ActorToSpawn, Destination, SpawnRotation);
	}
}

// Triggered every frame when the input is held down
void AZombieProjectPlayerController::OnSetDestinationTriggered()
{
	// We flag that the input is being pressed
	FollowTime += GetWorld()->GetDeltaSeconds();
	
	// We look for the location in the world where the player has pressed the input
	FHitResult Hit;
	bool bHitSuccessful = false;
	if (bIsTouch)
	{
		bHitSuccessful = GetHitResultUnderFinger(ETouchIndex::Touch1, ECollisionChannel::ECC_Visibility, true, Hit);
	}
	else
	{
		bHitSuccessful = GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, Hit);
	}

	// If we hit a surface, cache the location
	if (bHitSuccessful)
	{
		CachedDestination = Hit.Location;
	}
	
	// Move towards mouse pointer or touch
	APawn* ControlledPawn = GetPawn();
	if (ControlledPawn != nullptr)
	{
		FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection, 1.0, false);
	}
}

void AZombieProjectPlayerController::OnSetDestinationReleased()
{
	// If it was a short press
	if (FollowTime <= ShortPressThreshold)
	{
		// We move there and spawn some particles
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, CachedDestination);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, FXCursor, CachedDestination, FRotator::ZeroRotator, FVector(1.f, 1.f, 1.f), true, true, ENCPoolMethod::None, true);
	}

	FollowTime = 0.f;
}

// Triggered every frame when the input is held down
void AZombieProjectPlayerController::OnTouchTriggered()
{
	bIsTouch = true;
	OnSetDestinationTriggered();
}

void AZombieProjectPlayerController::OnTouchReleased()
{
	bIsTouch = false;
	OnSetDestinationReleased();
}

void AZombieProjectPlayerController::OnMoveForward() {
	APawn* camPawn = GetPawn();
	if (camPawn != nullptr) {
		FVector Direction = camPawn->GetActorForwardVector();
		camPawn->AddMovementInput(Direction, 1.0f);
	}
}
void AZombieProjectPlayerController::OnMoveBackward() {
	APawn* camPawn = GetPawn();
	if (camPawn != nullptr) {
		FVector Direction = camPawn->GetActorForwardVector();
		camPawn->AddMovementInput(Direction, -1.0f);
	}
}
void AZombieProjectPlayerController::OnMoveLeft() {
	APawn* camPawn = GetPawn();
	if (camPawn != nullptr) {
		FVector Direction = camPawn->GetActorRightVector();
		camPawn->AddMovementInput(Direction, -1.0f);
	}
}
void AZombieProjectPlayerController::OnMoveRight() {
	APawn* camPawn = GetPawn();
	if (camPawn != nullptr) {
		FVector Direction = camPawn->GetActorRightVector();
		camPawn->AddMovementInput(Direction, 1.0f);
	}}