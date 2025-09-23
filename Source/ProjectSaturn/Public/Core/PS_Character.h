// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "PS_Character.generated.h"

class UPS_AttributeSet;
class UPS_AbilitySystemComponent;
class APS_Prop_Base;
class APS_Prop_RechargingCapsule;

UCLASS()
class PROJECTSATURN_API APS_Character : public ACharacter, public IAbilitySystemInterface
{
    GENERATED_BODY()

public:
    APS_Character(const FObjectInitializer& ObjectInitializer);

    UFUNCTION(BlueprintCallable)
    void Interact();

    // Imitates ending of interaction process with prop.
    UFUNCTION(BlueprintCallable)
    void StartRespawnSequence();

    // Plays Death AnimMontage.
    UFUNCTION(BlueprintCallable)
    void StartDeathSequence();

    UFUNCTION(BlueprintPure)
    bool IsInteracting() const { return bIsInteracting; }

    UFUNCTION(BlueprintCallable)
    void TurnWithCursor(const bool bShouldTurn) { bShouldFollowCursor = bShouldTurn; };

    UFUNCTION(BlueprintPure)
    UAnimMontage* GetDeathAnimMontage() const { return DeathAnimation; };
    
    UFUNCTION()
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    UFUNCTION(BlueprintPure)
    bool IsDead() const { return bIsDead; }

    UFUNCTION(BlueprintCallable)
    void Move(const FInputActionValue& Value);
    
    UFUNCTION(BlueprintCallable)
    void Run(const FInputActionValue& Value);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATTRIBUTES|Debug")
    bool DisableSpawnAnimation = false;
    UPROPERTY(BlueprintReadWrite)
    bool bIsInteracting = false;
    UPROPERTY(BlueprintReadWrite)
    bool bIsDead = false;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    UAnimMontage* DeathAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATTRIBUTES")
    float InteractionRadius = 200.f;
    UPROPERTY(BlueprintReadWrite)
    bool bShouldFollowCursor = false;

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
    UPS_AbilitySystemComponent* AbilitySystemComponent;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATTRIBUTES|GAS")
    UPS_AttributeSet* AttributeSet;

    UPROPERTY(BlueprintReadWrite)
    APS_Prop_Base* NearbyInteractableProp; // stores last overlapped interactable prop
    UPROPERTY()
    FTimerHandle MontageDurationTimer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ATTRIBUTES")
    float MaxRunSpeed = 600.f;
    UPROPERTY(BlueprintReadWrite)
    float MaxWalkSpeedCached = 600.f;
    
    UPROPERTY(EditAnywhere, Category = "ATTRIBUTES|Energy Consumption")
    float EnergyForWalking = 0.01;
    UPROPERTY(EditAnywhere, Category = "ATTRIBUTES|Energy Consumption")
    float EnergyForRunning = 0.02;

    // REPLICATED VARIABLES AND METHODS
    float ReplicationInterval = 0.02f; // in seconds
    
    UPROPERTY(Replicated)
    bool IsRunning = false;
    
    UPROPERTY(ReplicatedUsing = OnRep_ReplicatedRotation)
    FRotator ReplicatedRotation;
    UFUNCTION()
    void OnRep_ReplicatedRotation();
    UFUNCTION(Server, Unreliable)
    void Server_UpdateRotation(const FRotator NewRotation);

    FTimerHandle ReplicationTimer;
    UFUNCTION(BlueprintCallable)
    void DoReplication();

    // ------------------------------------
    
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

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

    UFUNCTION()
    void ApplyEnergyDrainEffect(const float EnergyDrainAmount) const;

    UFUNCTION(Server, Reliable)
    void Server_Run(bool bWantsToRun);
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_Run(bool bWantsToRun);

    UFUNCTION()
    void RotateToMouseCursor();

    virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

};