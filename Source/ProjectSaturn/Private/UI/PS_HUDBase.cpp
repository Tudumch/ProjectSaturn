// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PS_HUDBase.h"

#include "Blueprint/UserWidget.h"

void APS_HUDBase::BeginPlay()
{
    Super::BeginPlay();

    GetOwningPlayerController()->SetInputMode(FInputModeGameOnly());
    WidgetHUD = CreateWidget<UUserWidget>(GetWorld(), WidgetHUDClass);
    if (WidgetHUD) WidgetHUD->AddToViewport();
}