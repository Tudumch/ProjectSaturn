// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PS_Prop_Base.h"

#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

// Sets default values
APS_Prop_Base::APS_Prop_Base()
{
	PrimaryActorTick.bCanEverTick = false;
    
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
    
    TextTooltip = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextTooltip"));
    TextTooltip->SetupAttachment(RootComponent);
    TextTooltip->SetWidgetSpace(EWidgetSpace::Screen);
}

void APS_Prop_Base::StartInteract(ACharacter* Character)
{
    if (!Character) return;
}

void APS_Prop_Base::StopInteract()
{
}

void APS_Prop_Base::ShowTooltip(const bool Value)
{
    TextTooltip->SetVisibility(Value);
}

void APS_Prop_Base::BeginPlay()
{
	Super::BeginPlay();

    ShowTooltip(false);
}