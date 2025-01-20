// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PS_CharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_CharacterBase : public ACharacter
{
	GENERATED_BODY()
    
public:
    APS_CharacterBase(const FObjectInitializer& ObjectInitializer);

protected:
    UPROPERTY()
    APlayerController* PlayerController;

    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class USpringArmComponent* SpringArm;
    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class UCameraComponent* Camera;
};
