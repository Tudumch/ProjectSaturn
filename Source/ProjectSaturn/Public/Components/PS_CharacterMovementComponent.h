// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PS_CharacterMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_CharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Move(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Look(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void Run();

protected:
    UPROPERTY()
    APawn* OwnerPawn;
    
    UPROPERTY()
    APlayerController* PlayerController;
    
    UPROPERTY(EditAnywhere, Category = "Character Movement: Walking")
    float MaxRunSpeed = 600;
    
    UPROPERTY()
    float MaxWalkSpeedCached;
    
    UPROPERTY()
    bool IsRunning = false;

    void BeginPlay() override;
};
