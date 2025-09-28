// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotifies/SendGameplayEvent.h"
#include "AbilitySystemBlueprintLibrary.h"

void USendGameplayEvent::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (!MeshComp->GetOwner() || !UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner())) return;
    
    UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MeshComp->GetOwner(), EventTag, FGameplayEventData());
}