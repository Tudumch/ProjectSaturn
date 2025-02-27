// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PS_WeaponComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTSATURN_API UPS_WeaponComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPS_WeaponComponent();

    UFUNCTION(BlueprintCallable)
    void AttackBaseOnPressed();
    UFUNCTION(BlueprintCallable)
    void AttackBaseOnReleased();

protected:
    UPROPERTY(BlueprintReadWrite)
    class UPS_AnimInstance* AnimInstance = nullptr;

    virtual void BeginPlay() override;

};