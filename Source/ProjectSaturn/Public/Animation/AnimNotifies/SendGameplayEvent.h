// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SendGameplayEvent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API USendGameplayEvent : public UAnimNotify
{
	GENERATED_BODY()

public:
    virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

    UPROPERTY(EditAnywhere)
    FGameplayTag EventTag;
};
