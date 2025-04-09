// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


