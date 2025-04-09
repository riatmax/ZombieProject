// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera.generated.h"

UCLASS(Blueprintable)
class ZOMBIEPROJECT_API ACamera : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACamera();

	//FORCEINLINE class UCameraComponent* GetCameraComponent() const { return Camera; }

public:	
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	
};
