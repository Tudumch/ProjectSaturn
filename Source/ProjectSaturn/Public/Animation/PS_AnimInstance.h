// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PS_AnimInstance.generated.h"

class APS_Prop_Base;
/**
 * 
 */
UCLASS()
class PROJECTSATURN_API UPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable)
    float StartInteractionWithProp(APS_Prop_Base* &Prop); // returns AnimMontage length
    UFUNCTION(BlueprintCallable)
    float EndInteractionWithProp(APS_Prop_Base* &Prop); // returns AnimMontage length
    
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float Speed;

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
