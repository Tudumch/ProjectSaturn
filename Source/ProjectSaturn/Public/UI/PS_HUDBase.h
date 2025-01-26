// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PS_HUDBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_HUDBase : public AHUD 
{
	GENERATED_BODY()


protected:
    UPROPERTY(EditAnywhere)
    TSubclassOf<UUserWidget> WidgetHUDClass;

    UPROPERTY()
    UUserWidget* WidgetHUD;

    virtual void BeginPlay() override;
};
