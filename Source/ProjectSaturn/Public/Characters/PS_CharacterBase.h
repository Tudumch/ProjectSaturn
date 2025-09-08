// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PS_CharacterBase.generated.h"

class UPS_AttributeSet;
class UPS_AbilitySystemComponent;
class APS_Prop_Base;
class APS_Prop_RechargingCapsule;

UCLASS()
class PROJECTSATURN_API APS_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    APS_CharacterBase(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void Interact();

    // Imitates ending of interaction process with prop. It used when the player respawns.
    UFUNCTION(BlueprintCallable)
    void StartRespawnSequence();

    // Plays Death AnimMontage.
    UFUNCTION(BlueprintCallable)
    void StartDeathSequence();

    UFUNCTION(BlueprintPure)
    bool IsInteracting() const { return bIsInteracting; }

    UFUNCTION(BlueprintPure)
    UAnimMontage* GetDeathAnimMontage() const { return DeathAnimation; };
    
    UFUNCTION()
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug")
    bool DisableSpawnAnimation = false;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInteracting = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
    UAnimMontage* DeathAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
    float InteractionRadius = 200.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USpringArmComponent* SpringArm;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UCameraComponent* Camera;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class USphereComponent* InteractionRadiusSphere;
    UPROPERTY(BlueprintReadWrite)
    class UPS_AnimInstance* AnimInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UPS_WeaponComponent* WeaponComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPS_AbilitySystemComponent* PS_AbilitySystemComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UPS_AttributeSet* PS_AttributeSet;

    UPROPERTY()
    APS_Prop_Base* NearbyInteractableProp; // stores last overlapped interactable prop
    UPROPERTY()
    FTimerHandle MontageDurationTimer;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void StartInteraction();
    UFUNCTION(BlueprintCallable)
    void EndInteraction();
    UFUNCTION()
    void OnFinishPlayStartingAnimMontage();
    UFUNCTION()
    void OnFinishPlayEndingAnimMontage();
    UFUNCTION(BlueprintCallable)
    virtual void OnZeroHealthEnergy(AActor* Actor);

};