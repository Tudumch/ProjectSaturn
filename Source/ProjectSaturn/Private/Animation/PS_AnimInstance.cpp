// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PS_AnimInstance.h"


void UPS_AnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
}

void UPS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    Speed = GetOwningActor()->GetVelocity().Length();
}