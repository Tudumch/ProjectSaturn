// Fill out your copyright notice in the Description page of Project Settings.


#include "Props/PS_Prop_Base.h"

#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/Character.h"

// Sets default values
APS_Prop_Base::APS_Prop_Base()
{
	PrimaryActorTick.bCanEverTick = false;
    
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StaticMesh;
    
    TextTooltip = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextTooltip"));
    TextTooltip->SetupAttachment(RootComponent);
}

void APS_Prop_Base::Interact(ACharacter* Character)
{
    USkeletalMeshComponent* Mesh = Character->GetMesh();
    if (!Mesh) return;

    if (AnimationStorage.InteractionAnimation)
    Mesh->PlayAnimation(AnimationStorage.InteractionAnimation, true);
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


