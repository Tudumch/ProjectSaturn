// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PS_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float Speed;

    void NativeInitializeAnimation() override;
    void NativeUpdateAnimation(float DeltaSeconds) override;
};
