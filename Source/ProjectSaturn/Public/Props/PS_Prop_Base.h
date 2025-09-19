// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PS_Prop_Base.generated.h"

// Contains all character's animations needed to interact with the props
USTRUCT(BlueprintType) 
struct FInteractionAnimationsStorage
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* StartingInteractionAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* InteractionAnimation;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    class UAnimMontage* EndingInteractionAnimation;
};

UCLASS()
class PROJECTSATURN_API APS_Prop_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	APS_Prop_Base();
    
    UFUNCTION(BlueprintCallable)
    virtual void StartInteract(ACharacter* Character);
    UFUNCTION(BlueprintCallable)
    virtual void StopInteract();
    UFUNCTION()
    virtual void ShowTooltip(const bool Value);
    UFUNCTION(BlueprintPure)
    FTransform GetAnimInteractionPointTransforms() const { return AnimationInteractionPoint->GetComponentTransform(); };
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
    FInteractionAnimationsStorage AnimationStorage;
    
protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneComponent* Root;
    UPROPERTY()
    ACharacter* InteractingCharacter = nullptr;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    USceneComponent* AnimationInteractionPoint;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UStaticMeshComponent* StaticMesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UWidgetComponent* TextTooltip;
    
	virtual void BeginPlay() override;
};
