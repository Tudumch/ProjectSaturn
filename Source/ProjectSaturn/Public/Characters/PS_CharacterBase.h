// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PS_CharacterBase.generated.h"

class APS_Prop_Base;
class APS_Prop_RechargingCapsule;
/**
 * 
 */
UCLASS()
class PROJECTSATURN_API APS_CharacterBase : public ACharacter
{
    GENERATED_BODY()
    
public:
    APS_CharacterBase(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void Interact();

    // Imitates ending of interaction process with prop. Is used when the player respawns.
    UFUNCTION(BlueprintCallable)
    void StartRespawnSequence();

    UFUNCTION(BlueprintPure)
    bool IsInteracting() const { return bIsInteracting; } 

protected:
    UPROPERTY()
    APlayerController* PlayerController;



    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class USpringArmComponent* SpringArm;
    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class UCameraComponent* Camera;
    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class USphereComponent* InteractionRadius;
    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class UPS_EnergyComponent* EnergyComponent;
    UPROPERTY (EditAnywhere, BlueprintReadWrite)
    class UPS_HealthComponent* HealthComponent;

    UPROPERTY()
    class APS_Prop_Base* NearbyInteractableProp; // stores last overlapped interactable prop
    UPROPERTY(BlueprintReadWrite)
    class UPS_AnimInstance* AnimInstance;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInteracting = false;
    UPROPERTY()
    FTimerHandle MontageDurationTimer;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UFUNCTION(BlueprintCallable)
    void StartInteraction();
    UFUNCTION(BlueprintCallable)
    void EndInteraction();
    UFUNCTION()
    void OnFinishPlayStartingAnimMontage();
    UFUNCTION()
    void OnFinishPlayEndingAnimMontage();
};
