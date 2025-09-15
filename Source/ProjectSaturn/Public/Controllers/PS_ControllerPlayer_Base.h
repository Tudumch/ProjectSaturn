// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "PS_ControllerPlayer_Base.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_ControllerPlayer_Base : public APlayerController
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    class APS_CharacterBase* PS_CharacterBase;
    UPROPERTY()
    class APS_GameModeBase* GameModeBase;
    UPROPERTY()
    class UPS_CharacterMovementComponent* PS_CharacterMovementComponent;
    UPROPERTY()
    class UPS_WeaponComponent* WeaponComponent;

    UPROPERTY(EditAnywhere, Category = "Input")
    class UInputMappingContext* InputMappingContext;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void LookAction(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void MoveAction(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void SprintAction(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void InteractAction();
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void AttackBaseAction(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void SaveAction();
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void LoadAction();

    // Debug cheats
    UPROPERTY(EditAnywhere, Category = "Input")
    UInputAction* DebugAddHPEnergyIA;

    // Increase and decrease HP and Energy. Bound to "+" and "-" hotkeys
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void DebugAddHPEnergyAction(const FInputActionValue& Value);

private:
    void DefineCoreVariables();

};