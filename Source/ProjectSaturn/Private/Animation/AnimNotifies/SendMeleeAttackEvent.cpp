// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifies/SendMeleeAttackEvent.h"

#include "Interfaces/AttackerMelee.h"

void USendMeleeAttackEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (IAttackerMelee* AttackerMelee = Cast<IAttackerMelee>(MeshComp->GetOwner()))
        AttackerMelee->DoMeleeAttack();
}