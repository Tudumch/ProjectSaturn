// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Library/PS_Enums.h"
#include "PS_AnimInstance.generated.h"

class APS_Prop_Base;

UCLASS()
class PROJECTSATURN_API UPS_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bIsAttacking = false;
    
    UFUNCTION(BlueprintCallable)
    float StartInteractionWithProp(APS_Prop_Base* &Prop); // returns AnimMontage length
    UFUNCTION(BlueprintCallable)
    float EndInteractionWithProp(APS_Prop_Base* &Prop); // returns AnimMontage length
    UFUNCTION(BlueprintCallable)
    void SetCurrentWeaponType(const EWeaponType Type) {CurrentWeaponType = Type;}
    UFUNCTION(BlueprintPure)
    EWeaponType GetCurrentWeaponType() const {return CurrentWeaponType;}
    
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
    float Speed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
    FRotator AimRotation;
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    EWeaponType CurrentWeaponType;

    UPROPERTY()
    APawn* Owner = nullptr;

    virtual void NativeInitializeAnimation() override;
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
