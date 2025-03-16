// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PS_AnimInstance.h"

#include "Props/PS_Prop_Base.h"


float UPS_AnimInstance::StartInteractionWithProp(APS_Prop_Base* &Prop)
{
    if (!Prop) return 0;
    return Montage_Play(Prop->AnimationStorage.StartingInteractionAnimation);
}

float UPS_AnimInstance::EndInteractionWithProp(APS_Prop_Base* &Prop)
{
    if (!Prop) return 0;
    return Montage_Play(Prop->AnimationStorage.EndingInteractionAnimation);
}

void UPS_AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    
    Owner = Cast<APawn>(GetOwningActor());
}

void UPS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    if (!Owner) return;
    Speed = GetOwningActor()->GetVelocity().Length();

    AimRotation = Owner->GetControlRotation() - Owner->GetActorRotation();
    // fix for degree-flips:
    AimRotation.Pitch > 180 ? AimRotation.Pitch -= 360 : AimRotation.Pitch;
    AimRotation.Yaw > 180 ? AimRotation.Yaw -= 360 : AimRotation.Yaw;

}