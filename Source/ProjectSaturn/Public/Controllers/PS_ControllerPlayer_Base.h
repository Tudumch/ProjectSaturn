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
    class UPS_CharacterMovementComponent* PS_CharacterMovementComponent;

     UPROPERTY (EditAnywhere, Category = "Input")
        class UInputMappingContext* InputMappingContext;

    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* PauseIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* LookIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* MoveIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* RunIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* InteractIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* SaveIA;
    UPROPERTY (EditAnywhere, Category = "Input")
    UInputAction* LoadIA;

    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void Look(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void Move(const FInputActionValue& Value);
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void ToggleRun();
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void Interact();
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void Save();
    UFUNCTION(BlueprintCallable, Category = "Input")
    virtual void Load();

private:
    void DefineCoreVariables();

};

