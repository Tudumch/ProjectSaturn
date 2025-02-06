// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PS_CharacterBase.generated.h"

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

    UFUNCTION(BlueprintCallable)
    bool IsInteracting() { return bIsInteracting; }

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

    UPROPERTY()
    class APS_Prop_Base* NearbyInteractableProp; // stores last overlapped interactable prop
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsInteracting = false;
    UPROPERTY()
    FTimerHandle MontageDurationTimer;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
    UFUNCTION(BlueprintCallable)
    void OnInteractionRadiusOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // ---------------------------------
    // Interaction with props section
    UFUNCTION(BlueprintCallable)
    void StartInteraction();
    UFUNCTION(BlueprintCallable)
    void EndInteraction();
    UFUNCTION(BlueprintCallable)
    void Interact_Starting();
    UFUNCTION(BlueprintCallable)
    void Interact_InProgress();
    UFUNCTION(BlueprintCallable)
    void Interact_Ending();
    UFUNCTION()
    void OnFinishPlayingEndingAnimMontage();
    // ---------------------------------
};
