// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SendMeleeAttackEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API USendMeleeAttackEvent : public UAnimNotify
{
	GENERATED_BODY()

    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
